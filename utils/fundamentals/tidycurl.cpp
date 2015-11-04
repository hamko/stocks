/* 
 * gcc -Wall -I/usr/local/include tidycurl.c -lcurl -ltidy -o tidycurl
 */ 

#include <stdio.h>
#include <tidy.h>
#include <buffio.h>
#include <curl/curl.h>

#include <sstream>
#include <iostream>

#include "list.h"

//#define DEBUG
using namespace std;

// コールバック
uint write_cb(char *in, uint size, uint nmemb, TidyBuffer *out);
// 2,000みたいな数字をdoubleに
double comma_atof(const char* bp);
// 表示用関数，普段は使わない
void dumpNode(TidyDoc doc, TidyNode tnod, int indent);
// 処理関数
void extractRealtimeStockInfo(TidyDoc doc, TidyNode tnod, int indent);

/* curl write callback, to fill tidy's input buffer...  */ 
uint write_cb(char *in, uint size, uint nmemb, TidyBuffer *out)
{
    uint r;
    r = size * nmemb;
    tidyBufAppend( out, in, r );
    return(r);
}

double comma_atof(const char* bp)
{
    char nocommaprice[32] = {};
    char* comma = strchr((char*)bp, ',');
    if (comma) {
        strncpy(nocommaprice, bp, (size_t)comma-(size_t)bp);
        strncpy(nocommaprice+((size_t)comma-(size_t)bp), comma+1, strlen(bp)-((size_t)comma-(size_t)bp)-1);
    } else {
        strcpy(nocommaprice, bp);
    }

    return atof(nocommaprice);
}

/********************************************/
// 妥当株価の抽出
/********************************************/
double price_now = 0;
int price_num = 0;
double f_price[3];
void extractRealtimeStockInfoRec(TidyDoc doc, TidyNode tnod, int indent )
{
    TidyNode child;
    for ( child = tidyGetChild(tnod); child; child = tidyGetNext(child) )
    {
        ctmbstr name = tidyNodeGetName( child );
        if ( name ) // HTML tag 
        {
            TidyAttr attr;
            // タグ名
#ifdef DEBUG
            int i;
            for (i = 0; i < indent; i++) printf(" ");
            printf("%s Tag (", name);
#endif
            // 属性名
            for ( attr=tidyAttrFirst(child); attr; attr=tidyAttrNext(attr) ) {
#ifdef DEBUG
                printf("%s", tidyAttrName(attr));
                tidyAttrValue(attr)?printf("=\"%s\" ", tidyAttrValue(attr)):printf(" ");
#endif
                // Stock Price
                // font class="medo_value"の検索
                if (name && !strcmp(name, "font") && !strcmp(tidyAttrName(attr), "class") && !strcmp(tidyAttrValue(attr), "medo_value")) {
                    TidyNode child_child = tidyGetChild(child);
                    TidyBuffer buf;
                    tidyBufInit(&buf);
                    tidyNodeGetText(doc, child_child, &buf);
                    f_price[price_num++] = comma_atof((char*)buf.bp); // Price
                }
                // td class="tbl_kabu"
                if (name && !strcmp(name, "td") && !strcmp(tidyAttrName(attr), "class") && !strcmp(tidyAttrValue(attr), "tbl_kabu")) {
                    TidyNode child_child = tidyGetChild(child);
                    TidyBuffer buf;
                    tidyBufInit(&buf);
                    tidyNodeGetText(doc, child_child, &buf);
                    price_now = comma_atof((const char*)buf.bp); // Price
                }

            }
#ifdef DEBUG
            printf(")\n");
#endif
        } else { /* text, cdata, etc... */ 
            TidyBuffer buf;
            tidyBufInit(&buf);
            tidyNodeGetText(doc, child, &buf);
#ifdef DEBUG
            int i;
            for (i = 0; i < indent; i++) printf(" ");
            printf("%s\n", buf.bp?(char *)buf.bp:"");
#endif
            tidyBufFree(&buf);
        }
        extractRealtimeStockInfoRec( doc, child, indent + 4 ); /* recursive */ 
    }
}


// 妥当株価の抽出
void extractRealtimeStockInfo(TidyDoc doc, TidyNode tnod, int indent ) {
    price_now = 0;
    price_num = 0;
    for (int i = 0; i < 3; i++)
        f_price[i] = 0;

    extractRealtimeStockInfoRec(doc, tnod, indent);
}

void extractExpectedStockPrice(const char* url)
{
    CURL *curl;
    char curl_errbuf[CURL_ERROR_SIZE];
    TidyDoc tdoc;
    TidyBuffer docbuf = {0};
    TidyBuffer tidy_errbuf = {0};
    int err;

    
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_errbuf);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);

    tdoc = tidyCreate();
    tidySetCharEncoding(tdoc, (const char*)"utf8");
    tidyOptSetBool(tdoc, TidyForceOutput, yes); /* try harder */ 
    tidyOptSetInt(tdoc, TidyWrapLen, 4096);
    tidySetErrorBuffer( tdoc, &tidy_errbuf );
    tidyBufInit(&docbuf);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &docbuf);
    err=curl_easy_perform(curl);
    if ( !err ) {
        err = tidyParseBuffer(tdoc, &docbuf); /* parse the input */ 
        if ( err >= 0 ) {
            err = tidyCleanAndRepair(tdoc); /* fix any problems */ 
            if ( err >= 0 ) {
                err = tidyRunDiagnostics(tdoc); /* load tidy error buffer */ 
                if ( err >= 0 ) {
                    //                        dumpNode( tdoc, tidyGetRoot(tdoc), 0 ); /* walk the tree */ 
                    extractRealtimeStockInfo( tdoc, tidyGetRoot(tdoc), 0 ); /* walk the tree */ 
                    fprintf(stderr, "%s\n", tidy_errbuf.bp); /* show errors */ 
                }
            }
        }
    }
    else
        fprintf(stderr, "%s\n", curl_errbuf);


    /* clean-up */ 
    curl_easy_cleanup(curl);
    tidyBufFree(&docbuf);
    tidyBufFree(&tidy_errbuf);
    tidyRelease(tdoc);
}

/********************************************/
// アナリストとレーティングの抽出
/********************************************/
int rating_counter = 0;
double rating[5] = {};
int rating_num[2] = {};
int rating_num_counter = 0;
bool lock = 1;
void extractAnalistNumRec(TidyDoc doc, TidyNode tnod, int indent )
{
    TidyNode child;
    for ( child = tidyGetChild(tnod); child; child = tidyGetNext(child) )
    {
        ctmbstr name = tidyNodeGetName( child );
        if ( name ) // HTML tag 
        {
            TidyAttr attr;
            // タグ名
#ifdef DEBUG
            int i;
            for (i = 0; i < indent; i++) printf(" ");
            printf("%s Tag (", name);
#endif
            // 属性名
            for ( attr=tidyAttrFirst(child); attr; attr=tidyAttrNext(attr) ) {
#ifdef DEBUG
                printf("%s", tidyAttrName(attr));
                tidyAttrValue(attr)?printf("=\"%s\" ", tidyAttrValue(attr)):printf(" ");
#endif
                // Stock Price
                // th class="history"の検索
                if (name && !strcmp(name, "th") && !strcmp(tidyAttrName(attr), "class") && !strcmp(tidyAttrValue(attr), "history")) {
                    lock = 0;
                }
                // td class="tbl_kabu"
                if (name && !strcmp(name, "td") && !strcmp(tidyAttrName(attr), "class") && !strcmp(tidyAttrValue(attr), "right") && !lock) {
                    TidyNode child_child = tidyGetChild(child);
                    TidyBuffer buf;
                    tidyBufInit(&buf);
                    tidyNodeGetText(doc, child_child, &buf);
                    rating[rating_counter++] = atof((const char*)buf.bp); // Price
                    lock = 1;
                }

            }

            if (name && !strcmp(name, "p")) {
                TidyNode child_child = tidyGetChild(child);
                TidyBuffer buf;
                tidyBufInit(&buf);
                tidyNodeGetText(doc, child_child, &buf);
                if (strstr((const char*)buf.bp, "アナリスト数")) {
                    stringstream ss((const char*)buf.bp);
                    string gabage, num_str;
                    ss >> gabage >> num_str;
                    rating_num[rating_num_counter++] = atoi(num_str.c_str());
                }
            }
#ifdef DEBUG
            printf(")\n");
#endif
        } else { /* text, cdata, etc... */ 
            TidyBuffer buf;
            tidyBufInit(&buf);
            tidyNodeGetText(doc, child, &buf);
#ifdef DEBUG
            int i;
            for (i = 0; i < indent; i++) printf(" ");
            printf("%s\n", buf.bp?(char *)buf.bp:"");
#endif
            tidyBufFree(&buf);
        }
        extractAnalistNumRec( doc, child, indent + 4 ); /* recursive */ 
    }
}


// 妥当株価の抽出
void extractAnalistNumStart(TidyDoc doc, TidyNode tnod, int indent ) {
    lock = 1;
    rating_counter = 0;
    for (int i = 0; i < 5; i++) 
        rating[i] = 0;
    for (int i = 0; i < 2; i++)
        rating_num[i] = 0;
    rating_counter = 0;
    rating_num_counter = 0;

    extractAnalistNumRec(doc, tnod, indent);
}

void extractAnalistNum(const char* url)
{
    CURL *curl;
    char curl_errbuf[CURL_ERROR_SIZE];
    TidyDoc tdoc;
    TidyBuffer docbuf = {0};
    TidyBuffer tidy_errbuf = {0};
    int err;


    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_errbuf);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);

    tdoc = tidyCreate();
    tidySetCharEncoding(tdoc, (const char*)"utf8");
    tidyOptSetBool(tdoc, TidyForceOutput, yes); /* try harder */ 
    tidyOptSetInt(tdoc, TidyWrapLen, 4096);
    tidySetErrorBuffer( tdoc, &tidy_errbuf );
    tidyBufInit(&docbuf);

    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &docbuf);
    err=curl_easy_perform(curl);
    if ( !err ) {
        err = tidyParseBuffer(tdoc, &docbuf); /* parse the input */ 
        if ( err >= 0 ) {
            err = tidyCleanAndRepair(tdoc); /* fix any problems */ 
            if ( err >= 0 ) {
                err = tidyRunDiagnostics(tdoc); /* load tidy error buffer */ 
                if ( err >= 0 ) {
                    //                        dumpNode( tdoc, tidyGetRoot(tdoc), 0 ); /* walk the tree */ 
                    extractAnalistNumStart( tdoc, tidyGetRoot(tdoc), 0 ); /* walk the tree */ 
                    fprintf(stderr, "%s\n", tidy_errbuf.bp); /* show errors */ 
                }
            }
        }
    }
    else
        fprintf(stderr, "%s\n", curl_errbuf);


    /* clean-up */ 
    curl_easy_cleanup(curl);
    tidyBufFree(&docbuf);
    tidyBufFree(&tidy_errbuf);
    tidyRelease(tdoc);
}

int stock_start = 1000;
int stock_end = 9999;
int main(int argc, char **argv )
{
#ifdef GET_ALL
    if (argc == 3) {
        stock_start = atoi(argv[1]);
        stock_end = atoi(argv[2]);
        fprintf(stderr, "from %d to %d\n", stock_start, stock_end);
        sleep(1);
    }
    for (int i = stock_start; i <= stock_end; i++) {
#endif
    for (int j = 0; j < sizeof(list) / sizeof(list[0]); j++) {
        int i = list[j];
        ostringstream os;
        os.str("");
        os << "http://kabuyoho.ifis.co.jp/index.php?action=tp1&sa=report_per&bcode=" << to_string((long long)i);
        extractExpectedStockPrice(os.str().c_str()); // PER
        if (!price_now)
            continue;

        cout << i << " " << (int)price_now << " " << (int)f_price[1] << " " << (int)(f_price[0] - f_price[1]) << " ";
        if (f_price[0] - f_price[1] != 0)
            cout << (price_now - f_price[1]) / (f_price[0] - f_price[1]) * 2 << " ";
        else 
            cout << 0 << " ";

        os.str("");
        os << "http://kabuyoho.ifis.co.jp/index.php?action=tp1&sa=report_pbr&bcode=" << to_string((long long)i);
        extractExpectedStockPrice(os.str().c_str()); // PBR
        cout << (int)f_price[1] << " " << (int)(f_price[0] - f_price[1]) << " ";
        if (f_price[0] - f_price[1] != 0)
            cout << (price_now - f_price[1]) / (f_price[0] - f_price[1]) * 2 << " ";
        else 
            cout << 0 << " ";

        os.str("");
        os << "http://kabuyoho.ifis.co.jp/index.php?action=tp1&sa=report_chg&bcode=" << to_string((long long)i);
        extractAnalistNum(os.str().c_str()); // PBR
//        cout << "|"  << rating_num[0] << "|" << rating_num[1] << endl;

        cout << rating_num[0] << " " << rating_num[1] << " " << rating[0] << " " << rating[1] << " " << rating[2] << " " << rating[3] << " " << rating[4] << " ";

        cout << endl;
    }
    return(0);
}

