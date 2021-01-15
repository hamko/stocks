# -*- coding: utf-8 -*-
import matplotlib.pyplot as plt
import pandas as pd

c = {
  "現金・債権": "#b2d8ff",
  "流動負債": "#fab1a0",
  "コモディティ": "#55efc4",
  "株式": "#fdcb6e",
  "Misaki": "#a29bfe",
  "Misaki 借金": "#ff7675",
  "純資産": "#dfe6e9"
}

length = 6000

def graph(ax, x, data, data_name, color1, color2):
    cum = 0
    for i in range(0, len(data))[::-1]:
        if data_name[i] in c:
            color = c[data_name[i]]
        else:
            color = "#ffffff"
        y = data[i]
        
        ax.bar(x, y, width=1.0, bottom=cum, color="{}".format(color), edgecolor="none")
        if y > 0:
            ax.text(x, cum+0.4*y, "{0} ({1})".format(data_name[i], int(y)), 
                    ha='center', va='bottom', fontdict={'family': 'IPAexGothic'}, fontsize=20)
        cum += y

def draw(fig, df, x, y, index):
    ax = fig.add_subplot(x, y, index)
    ax.set_xlim(-1, 1)
    ax.set_ylim(0, length)
    ax.axis('off')
    ax.tick_params(labelbottom=False, labelleft=False, left=False, bottom=False)

    pure_asset = sum(df.ass_value) - sum(df.lia_value)
    for index, item in df.iterrows():
        if item.lia == "純資産":
            df.loc[index, "lia_value"] = pure_asset
    print(pure_asset)
    ax.title.set_text("{} %".format(int(100.0* pure_asset/sum(df.ass_value))))

    graph(ax, -0.5, df.ass_value, df.ass, "#b2d8ff", "#7fbfff")
    graph(ax, 0.5, df.lia_value, df.lia, "#ffb2b2", "#ff7f7f")

fig = plt.figure()
fig.suptitle("Balance Sheet")

df = pd.read_csv('bs.csv', names=('ass', 'ass_value', 'lia', 'lia_value'), encoding="UTF-8")
print (df)
draw(fig, df, 1, 2, 1)

df = pd.read_csv('bs2.csv', 
                 names=('ass', 'ass_value', 'lia', 'lia_value'), encoding="UTF-8")
draw(fig, df, 1, 2, 2)

plt.show()
