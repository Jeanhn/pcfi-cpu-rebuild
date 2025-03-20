import torch
import torch.nn.functional as F
import torch_geometric.nn as pyg_nn
from torch_geometric.data import Data
import numpy as np


# 读取 cora.content 文件
def read_cora_content(file_path):
    with open(file_path, "r") as f:
        lines = f.readlines()
    features = []
    labels = []
    node_ids = []
    for line in lines:
        parts = line.strip().split("\t")
        node_id = int(parts[0])
        feature = [float(x) for x in parts[1:-1]]
        label = parts[-1]
        node_ids.append(node_id)
        features.append(feature)
        labels.append(label)
    unique_labels = sorted(set(labels))
    label_mapping = {label: idx for idx, label in enumerate(unique_labels)}
    labels = [label_mapping[label] for label in labels]
    features = torch.tensor(features, dtype=torch.float)
    labels = torch.tensor(labels, dtype=torch.long)
    node_id_mapping = {node_id: idx for idx, node_id in enumerate(node_ids)}
    return features, labels, node_id_mapping


# 读取 cora.cites 文件
def read_cora_cites(file_path, node_id_mapping):
    with open(file_path, "r") as f:
        lines = f.readlines()
    edge_index = []
    for line in lines:
        parts = line.strip().split("\t")
        source = int(parts[0])
        target = int(parts[1])
        if source in node_id_mapping and target in node_id_mapping:
            source_idx = node_id_mapping[source]
            target_idx = node_id_mapping[target]
            edge_index.append([source_idx, target_idx])
    edge_index = torch.tensor(edge_index, dtype=torch.long).t().contiguous()
    return edge_index


# 加载数据
content_path = r"C:\Users\jean\Desktop\pcfi-cpu\cora\cora.content"
cites_path = r"C:\Users\jean\Desktop\pcfi-cpu\cora\cora.cites"
features, labels, node_id_mapping = read_cora_content(content_path)
edge_index = read_cora_cites(cites_path, node_id_mapping)

# 划分训练集、验证集和测试集
num_nodes = features.size(0)
train_mask = torch.zeros(num_nodes, dtype=torch.bool)
val_mask = torch.zeros(num_nodes, dtype=torch.bool)
test_mask = torch.zeros(num_nodes, dtype=torch.bool)

train_size = int(num_nodes * 0.6)
val_size = int(num_nodes * 0.2)
test_size = num_nodes - train_size - val_size

train_indices = np.random.choice(num_nodes, train_size, replace=False)
remaining_indices = [i for i in range(num_nodes) if i not in train_indices]
val_indices = np.random.choice(remaining_indices, val_size, replace=False)
test_indices = [i for i in remaining_indices if i not in val_indices]

train_mask[train_indices] = True
val_mask[val_indices] = True
test_mask[test_indices] = True

# 创建 Data 对象
data = Data(
    x=features,
    y=labels,
    edge_index=edge_index,
    train_mask=train_mask,
    val_mask=val_mask,
    test_mask=test_mask,
)


# 定义图神经网络模型
class GNN(torch.nn.Module):
    def __init__(self):
        super(GNN, self).__init__()
        self.conv1 = pyg_nn.GCNConv(data.num_node_features, 16)
        self.conv2 = pyg_nn.GCNConv(16, len(set(labels.tolist())))

    def forward(self, data):
        x, edge_index = data.x, data.edge_index

        x = self.conv1(x, edge_index)
        x = F.relu(x)
        x = F.dropout(x, training=self.training)
        x = self.conv2(x, edge_index)

        return F.log_softmax(x, dim=1)


# 初始化模型、优化器和损失函数
model = GNN()
optimizer = torch.optim.Adam(model.parameters(), lr=0.01, weight_decay=5e-4)


# 训练模型
def train():
    model.train()
    optimizer.zero_grad()
    out = model(data)
    loss = F.nll_loss(out[data.train_mask], data.y[data.train_mask])
    loss.backward()
    optimizer.step()
    return loss.item()


# 测试模型
def test():
    model.eval()
    out = model(data)
    pred = out.argmax(dim=1)
    test_correct = pred[data.test_mask] == data.y[data.test_mask]
    test_acc = int(test_correct.sum()) / int(data.test_mask.sum())
    return test_acc


# 训练循环
for epoch in range(200):
    loss = train()
    if (epoch + 1) % 10 == 0:
        test_acc = test()
        print(f"Epoch: {epoch + 1}, Loss: {loss:.4f}, Test Acc: {test_acc:.4f}")
