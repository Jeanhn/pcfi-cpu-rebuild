import networkx as nx
import pandas as pd
from gensim.models import Word2Vec
from node2vec import Node2Vec
import torch
import torch.nn as nn
import torch.optim as optim
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import MultiLabelBinarizer
import numpy as np

# 数据路径配置
DATA_DIR = r"..\data\blogcatalog\data"
nodes_path = f"{DATA_DIR}/nodes.csv"
edges_path = f"{DATA_DIR}/edges.csv"
group_edges_path = f"{DATA_DIR}/group-edges.csv"
groups_path = f"{DATA_DIR}/groups.csv"


# 读取数据并构建图
def load_graph():
    # 读取边数据
    edges = pd.read_csv(edges_path, header=None, names=["source", "target"])

    # 创建无向图
    G = nx.Graph()
    for _, row in edges.iterrows():
        G.add_edge(row["source"], row["target"])
    return G


# 读取节点和群组的关联数据
def load_group_edges():
    group_edges = pd.read_csv(group_edges_path, header=None, names=["node", "group"])
    node_groups = group_edges.groupby("node")["group"].apply(list).to_dict()
    return node_groups


# 训练Node2Vec模型
def train_node2vec(G):
    # 初始化Node2Vec对象
    node2vec = Node2Vec(
        G,
        dimensions=128,  # 嵌入维度
        walk_length=30,  # 随机游走长度
        num_walks=200,  # 每个节点游走次数
        workers=4,  # 使用多线程加速
    )

    # 训练模型
    model = node2vec.fit(
        window=10,  # 上下文窗口大小
        min_count=1,  # 忽略低频节点
        batch_words=2048,  # 每批处理词数
    )

    return model


# 定义一个简单的多层感知机模型用于多标签分类
class MLP(nn.Module):
    def __init__(self, input_dim, output_dim):
        super(MLP, self).__init__()
        self.fc1 = nn.Linear(input_dim, 64)
        self.relu = nn.ReLU()
        self.fc2 = nn.Linear(64, output_dim)
        self.sigmoid = nn.Sigmoid()

    def forward(self, x):
        x = self.fc1(x)
        x = self.relu(x)
        x = self.fc2(x)
        x = self.sigmoid(x)
        return x


if __name__ == "__main__":
    # 加载图数据
    print("Loading graph...")
    G = load_graph()

    # 加载节点和群组的关联数据
    node_groups = load_group_edges()

    # 训练Node2Vec模型
    print("Training model...")
    model = train_node2vec(G)

    # 获取节点嵌入
    node_embeddings = {node: model.wv[node] for node in G.nodes()}

    # 准备数据
    X = []
    y = []
    for node, groups in node_groups.items():
        X.append(node_embeddings[node])
        y.append(groups)

    # 多标签二值化
    mlb = MultiLabelBinarizer()
    y = mlb.fit_transform(y)

    X = np.array(X)
    y = np.array(y)

    # 划分训练集和测试集
    X_train, X_test, y_train, y_test = train_test_split(
        X, y, test_size=0.2, random_state=42
    )

    # 转换为PyTorch张量
    X_train = torch.FloatTensor(X_train)
    y_train = torch.FloatTensor(y_train)
    X_test = torch.FloatTensor(X_test)
    y_test = torch.FloatTensor(y_test)

    # 初始化模型、损失函数和优化器
    input_dim = X_train.shape[1]
    output_dim = y_train.shape[1]
    model = MLP(input_dim, output_dim)
    criterion = nn.BCELoss()
    optimizer = optim.Adam(model.parameters(), lr=0.001)

    # 训练模型
    num_epochs = 100
    for epoch in range(num_epochs):
        optimizer.zero_grad()
        outputs = model(X_train)
        loss = criterion(outputs, y_train)
        loss.backward()
        optimizer.step()
        if (epoch + 1) % 10 == 0:
            print(f"Epoch [{epoch + 1}/{num_epochs}], Loss: {loss.item():.4f}")

    # 测试模型
    model.eval()
    with torch.no_grad():
        test_outputs = model(X_test)
        predicted = (test_outputs > 0.5).float()
        accuracy = (predicted == y_test).sum().item() / (y_test.numel())
        print(f"Test Accuracy: {accuracy:.4f}")

    # 示例预测
    sample_node_index = 0
    sample_node_embedding = X_test[sample_node_index].unsqueeze(0)
    with torch.no_grad():
        sample_output = model(sample_node_embedding)
        sample_predicted = (sample_output > 0.5).float()
        sample_predicted_groups = mlb.inverse_transform(sample_predicted.numpy())
        print(
            f"预测节点 {list(node_groups.keys())[sample_node_index]} 加入的群组: {sample_predicted_groups}"
        )
