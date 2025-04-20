## 目录内容
### ./py/blog_model.py
使用blog catalog数据集训练，预测node所属的群组

### ./py/cora_model.py
使用cora数据集训练，预测论文所属的分类

### ./data
各种数据集

## build
- 行数统计: 
    - find . -name "*.cc" -o -name "*.hh" | xargs wc -l
- 编译:
    - cmake ./CMakeLists.txt
    - make

## 原模型整体思路(节点特征补全)
- 对于数据集中, 某个node(x)本身缺失的某一项特征, 把数据集node分为两类: 缺失该feature的节点(missing node), 以及不确实该feature的节点(source node)
- 要根据source node的该列特征值, 对所有missing node的该列特征进行补全
- 论文假设, 根据node之间的edge关系, 距离source node集合越近的node产生的影响力越大
- 在初次迭代中, 所有缺失该feature的node的feature都被设为0
- 使用以下公式, 计算出第一次迭代的补全结果(α为节点间影响力参数, 一般设为0.8)
  - 补全的特征值 = [∑（源节点及邻居节点的该列特征值 × α 的（该节点到源节点集合最短距离 - 缺失节点到源节点集合最短距离）次方）] / [∑α 的（该节点到源节点集合最短距离 - 缺失节点到源节点集合最短距离）次方]
- 经过多次迭代, 补全出最终结果(经实验, 10次之后结果就比较准确了)

## 边特征补全思路
- 原论文的思路大概可以概括为以下公式:
  - fix(missing_node, source_node, edge, features) -> return missing_feature
- 对于节点连接的补全(判断两个node之间是否存在直接连接), 把节点连接也是为一种特征
- 比如cora数据集中, cora.content是一个描述节点特征的矩阵; 把节点连接特征做相应的转换后就成了图论中经常提到的邻接矩阵
- 根据邻接矩阵, 把节点分为missing_node和source_node两个合集; 0表示无连接, 1表示有连接;
- **最后对于edge**: edge用来计算missing node到source node的距离, 推出node的伪置信度; 不同数据集的处理也不一样
  - **cora数据集**:
    - 在补全cora.edge时, 把cora.content视作节点之间的边关系
    - 对于两个node间是否存在边, 利用cora.content中的特征计算出node的余弦值cosine
    - 计算出相同类型的node的笛卡尔cosine值平均数（大约0.08）
    - 计算总共的node的笛卡尔平均数（大约0.05）
    - 由于总体cosine平均数略大于不同类的cosine平均数，所以取界限为0.06
    - cosine大于0.06算作一条边，否则算作无边
  - **blog_catalog数据集**:
    - blog_catalog分为gourp_edges和edges两个文件, 分别描述了node加入了哪个group, 以及哪两个node之间存在连接
    - **补全group_edges**: 把node是否加入了group视作feature, edges直接使用(类比补全cora.content)
    - **补全edges**: edges作为邻接矩阵, edge加入的group视为节点内部特征, 然后计算cos, 计算平均数, 最后按略小于平均数为界限, 生成edge
- 总结为以下公式:
  - 一般数据集中, 有两份文件, 一份是node的连接, 另一份是node本身的特征(在blogcatalog中把node是否加入group视作本身特征)
  - 要补全连接, 把连接视作特征, 利用node内部特征计算出cosine余弦值, 经过四舍五入处理后把相似的节点(夹角小)视作有连接