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