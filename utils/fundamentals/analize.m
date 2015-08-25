M = dlmread("a");

# 妥当株価(PER)に対して現在の株価がどれくらいか
M_per_sub = (M(:,2)-M(:,3)).*(M(:,3)&M(:,3));
M_per_sub = [M_per_sub M_per_sub./M(:,2)];

# 妥当株価(PBR)に対して現在の株価がどれくらいか
M_pbr_sub = (M(:,2)-M(:,6)).*(M(:,6)&M(:,6));
M_pbr_sub = [M_pbr_sub M_pbr_sub./M(:,2)];

# のちのち面倒なので連結
M = [M M_per_sub, M_pbr_sub];

# 全PER, PBR基準株価
M_per = M(M(:, 5) != 0, :);
M_pbr = M(M(:, 8) != 0, :);
save M_per.dat M_per
save M_pbr.dat M_pbr

# 銘柄，PER妥当株価，PBR妥当株価，アナリスト人数，レーティング, PER妥当株価と現在との差の割合，PBR妥当株価と現在との差の割合
selection = [1, 5, 8, 9, 11, 17, 19]; 

#全てソートのキーは，対PER妥当株価騰落率で行なっている．

# 割安注目株(アナリスト5人以上, レーティング4.0以上, PBR, PERともに標準偏差-1.5以下)なもの
M_Economical = M(M(:, 5) < -1.5 & M(:, 8) < -1.5 & M(:, 9) > 4 & M(:, 11) > 4.0, selection);
M_Economical = sortrows(M_Economical, 6)

# 割高注目株(アナリスト5人以上, レーティング4.0以上, PBR, PERともに標準偏差-1.5以下)なもの
M_Expensive = M(M(:, 5) > +1.5 & M(:, 8) > +1.5 & M(:, 9) > 4 & M(:, 11) > 4.0, selection);
M_Expensive = sortrows(M_Expensive, 6)

M_GoodCompany = M(M(:, 11)>4.0 & M(:, 9) > 4, selection);
M_GoodCompany = sortrows(M_GoodCompany, 6)
 
save M_Economical.dat M_Economical
save M_Expensive.dat M_Expensive
save M_GoodCompany.dat M_GoodCompany


#hist(M(M(:,17)!=0, 5), 20); figure;       # PER妥当株価の差のσ
#hist(M(M(:,17)!=0, 17), 20); figure;      # PER妥当株価との差の割合
figure; hist(M_GoodCompany(:, 2), 20);     # 注目株のPER妥当株価の差のσ
figure; hist(M_GoodCompany(:, 6), 20);     # 注目株のPER妥当株価との差の割合
