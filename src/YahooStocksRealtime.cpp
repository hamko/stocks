#include "YahooStocksRealtime.hpp"
#include <sstream>
#include <iostream>

static uint write_cb(char *in, uint size, uint nmemb, TidyBuffer *out)
{
    uint r;
    r = size * nmemb;
    tidyBufAppend( out, in, r );
    return(r);
}

double YahooStocksRealtime::comma_atof(unsigned char* bp)
{
    char nocommaprice[32] = {};
    char* comma = strchr((char*)bp, ',');
    if (comma) {
        strncpy(nocommaprice, (const char*)bp, (size_t)comma-(size_t)bp);
        strncpy(nocommaprice+((size_t)comma-(size_t)bp), (const char*)comma+1, strlen((const char*)bp)-((size_t)comma-(size_t)bp)-1);
    } else {
        strcpy(nocommaprice, (const char*)bp);
    }

    return atof(nocommaprice);
}


YahooStocksRealtime::YahooStocksRealtime(int code, std::string market) 
{
    data.setPrices(-1);

    CURL *curl;
    char curl_errbuf[CURL_ERROR_SIZE];
    TidyDoc tdoc;
    TidyBuffer docbuf = {0};
    TidyBuffer tidy_errbuf = {0};
    int err = 0;

    std::stringstream ss;
    ss << code;
    std::string url = (std::string)"http://stocks.finance.yahoo.co.jp/stocks/detail/?code=" + ss.str() + "." + market;
//    std::cout << url << " #URL" << std::endl;
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
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
    if (curl_easy_perform(curl)) {
        fprintf(stderr, "curl error: %s\n", curl_errbuf);
        err = 1;
    }
    tidyParseBuffer(tdoc, &docbuf); /* parse the input */ 
    tidyCleanAndRepair(tdoc); /* fix any problems */ 
    tidyRunDiagnostics(tdoc); /* load tidy error buffer */ 
    fprintf(stderr, "%s\n", tidy_errbuf.bp); /* show errors */ 

    if (!err) 
        extractRealtimeStockInfo( tdoc, tidyGetRoot(tdoc), 0 ); /* walk the tree */ 

    /* clean-up */ 
    curl_easy_cleanup(curl);
    tidyBufFree(&docbuf);
    tidyBufFree(&tidy_errbuf);
    tidyRelease(tdoc);

}

/* Traverse the document tree */ 
void YahooStocksRealtime::extractRealtimeStockInfo(TidyDoc doc, TidyNode tnod, int indent )
{
    TidyNode child;
    for ( child = tidyGetChild(tnod); child; child = tidyGetNext(child) )
    {
        ctmbstr name = tidyNodeGetName( child );
        if ( name ) // HTML tag 
        {
            /*
               int i;
               for (i = 0; i < indent; i++) printf(" ");
               printf("%s Tag (", name);
               */

            TidyAttr attr;
            // 属性名
            for ( attr=tidyAttrFirst(child); attr; attr=tidyAttrNext(attr) ) {
                /*
                   printf("%s", tidyAttrName(attr));
                   tidyAttrValue(attr)?printf("=\"%s\" ", tidyAttrValue(attr)):printf(" ");
                   */

                // Stock Price
                if (name && !strcmp(name, "td") && !strcmp(tidyAttrName(attr), "class") && !strcmp(tidyAttrValue(attr), "stoksPrice")) {
                    child = tidyGetChild(child);
                    TidyBuffer buf;
                    tidyBufInit(&buf);
                    tidyNodeGetText(doc, child, &buf);

                    //                    std::cout << buf.bp << std::endl;
                    // 朝など
                    if (strstr((const char*)buf.bp, "---"))
                        data.setPrices(-1);
                    else
                        data.setPrices(comma_atof(buf.bp));
                }
            }
        } else { /* text, cdata, etc... */ 
            TidyBuffer buf;
            tidyBufInit(&buf);
            tidyNodeGetText(doc, child, &buf);

            /*
               int i;
               for (i = 0; i < indent; i++) printf(" ");
               printf("%s\n", buf.bp?(char *)buf.bp:"");
               */
            tidyBufFree(&buf);
        }
        extractRealtimeStockInfo( doc, child, indent + 2 ); /* recursive */ 
    }
}

double YahooStocksRealtime::getPrices(void)
{
    return data.getPrices();
}
