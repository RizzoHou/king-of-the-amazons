# 亚马逊女王 - 项目报告（中文精简版）

## 1. 项目简介

### 1.1 项目概述
**亚马逊女王**（King of the Amazons）是经典抽象策略棋盘游戏"亚马逊游戏"的C++实现，作为计算概论课程的大作业项目。

### 1.2 当前状态
- **阶段**: 第3阶段完成（图形化GUI已实现）
- **完成度**: 85%（第1、2、3阶段完成，第4阶段进行中）
- **版本**: 1.2.0
- **最后更新**: 2026年1月7日

## 2. 如何运行程序

### 2.1 系统要求
- **操作系统**: Windows、macOS或Linux
- **C++编译器**: GCC ≥ 9.0、Clang ≥ 10.0或MSVC ≥ 2019
- **构建系统**: CMake ≥ 3.16

### 2.2 快速安装指南

#### 使用CMake构建
```bash
cd king-of-the-amazons
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

#### 运行程序
```bash
# 图形模式（默认）
./build/amazons

# 文本模式
./build/amazons --text
```

### 2.3 测试程序
```bash
# 运行单元测试
cd build
./bin/unit_tests
```

### 2.4 SFML安装
图形界面需要SFML 3.0。根据平台安装：

**macOS**（使用Homebrew）：
```bash
brew install sfml
```

**Linux**（Ubuntu/Debian）：
```bash
sudo apt-get install libsfml-dev
```

**Windows**：
- 从[SFML官网](https://www.sfml-dev.org/download.php)下载
- 或使用vcpkg：`vcpkg install sfml`

**验证**：安装后CMake应自动找到SFML。如遇构建错误，请确保SFML已安装。

**注意**：文本模式（`--text`标志）无需SFML。

## 3. AI实现

### 3.1 AI系统概述
项目包含两种AI实现：

1. **BasicAI**: 内置贪心算法，基于移动性启发式
2. **BotzoneAI**: 外部bot003高级算法接口

### 3.2 BasicAI算法核心
```cpp
// 贪心算法选择最佳走法
Move BasicAI::getBestMove(const GameState& state) {
    std::vector<Move> legalMoves = state.getLegalMoves();
    Move bestMove;
    int bestScore = INT_MIN;
    
    for (const Move& move : legalMoves) {
        GameState nextState = state;
        nextState.makeMove(move);
        int score = evaluatePosition(nextState); // 移动性+中心控制+领地优势
        if (score > bestScore) {
            bestScore = score;
            bestMove = move;
        }
    }
    return bestMove;
}
```

### 3.3 外部AI集成
- **来源**: amazing-amazons项目的bot003算法
- **集成方式**: 进程间通信，3秒超时
- **技术报告**: [bot003_report_simplified_zh.md](https://github.com/RizzoHou/amazing-amazons/blob/main/docs/bots/reports/bot003_report_simplified_zh.md)

### 3.4 AI性能特征
- **BasicAI**: ~50毫秒/步，实时游戏
- **BotzoneAI**: 1-3秒/步，深度分析
- **内存使用**: 高效状态表示

## 4. 特色功能

### 4.1 双界面系统
- **图形模式**: 鼠标三步操作（选择亚马逊→选择目的地→选择箭）
- **文本模式**: 控制台6数字输入格式
- **切换方式**: `--text`或`--graphical`命令行参数

### 4.2 保存/加载系统
- **JSON序列化**: 完整游戏状态保存
- **游戏模式跟踪**: HUMAN_VS_HUMAN、HUMAN_VS_AI、AI_VS_AI
- **向后兼容**: 处理旧版保存文件

### 4.3 用户体验增强
- **继续游戏功能**: ESC自动保存，菜单显示继续按钮
- **非阻塞AI**: 后台计算，界面保持响应
- **可滚动加载屏幕**: 5个可见保存，支持滚动浏览
- **现代配色方案**: 蓝灰棋盘，彩色高亮反馈

### 4.4 键盘快捷键
- **R**: 重新开始游戏
- **U**: 撤销上一步
- **ESC**: 返回菜单（自动保存）

## 5. 项目架构

### 5.1 技术栈
- **语言**: C++17
- **图形库**: SFML 3.0
- **构建系统**: CMake 3.16+
- **测试框架**: Google Test（30个单元测试）
- **版本控制**: Git + Conventional Commits

### 5.2 设计模式
- **MVC架构**: 模型-视图-控制器分离
- **策略模式**: AI算法可互换
- **工厂模式**: 显示创建（文本/图形）
- **观察者模式**: 状态变更通知

## 6. 开发进度

### 6.1 阶段完成情况
- **✅ 阶段1**（12月17-23日）: 核心基础，人类对战
- **✅ 阶段2**（12月24-30日）: 完整游戏系统，AI对战，保存/加载
- **✅ 阶段3**（12月31日-1月6日）: 图形化GUI，鼠标交互
- **✅ 2026年1月更新**（1月7日）: 侧边选择、撤销修复、颜色协调
- **🟡 阶段4**（1月7-10日）: 完善功能，测试优化

### 6.2 验收准备状态
- **第一次验收**（12月30日）: ✅ 准备就绪
- **第二次验收**（1月9日）: 🟡 准备中
- **第三次验收**（1月10日）: 🟡 最终目标

## 7. 课程要求符合性

### 7.1 基本功能（6分）- ✅ 完全实现
1. **菜单系统**: 新游戏、加载、保存、退出
2. **棋盘显示**: 字符和图形两种方式
3. **完整游戏流程**: 可中途停止
4. **保存/加载**: JSON文件操作

### 7.2 AI能力（1分）- ✅ 完全实现
- **BasicAI**: 贪心算法，移动性启发式
- **BotzoneAI**: 外部高级算法集成
- **详细文档**: 本报告第3节

### 7.3 特色分（1.5分）- ✅ 完全实现
- **用户友好**: 直观的图形/文本界面
- **界面美观**: 现代配色，视觉反馈
- **功能完整**: 所有基本功能良好集成
- **额外特色**: 双界面、非阻塞AI、继续游戏、滚动加载

## 8. 测试验证

### 8.1 测试覆盖
- **30个单元测试**: 覆盖所有核心功能
- **测试类别**: 棋盘初始化、走法验证、AI算法、保存/加载
- **测试执行**: `./bin/unit_tests`

### 8.2 质量保证
- **编译检查**: 严格标志下零警告
- **内存检测**: Valgrind无泄漏
- **代码规范**: 一致命名和格式

## 9. 项目资源

### 9.1 代码仓库
- **GitHub**: https://github.com/RizzoHou/king-of-the-amazons
- **最新提交**: ebffedf8db9fcc3c0a33d50e2979c5838a49bfd4

### 9.2 文档目录
- **项目报告**: `docs/reports/`（本文件）
- **游戏手册**: `docs/manuals/game_manual.md`
- **实施计划**: `docs/implementation/`
- **记忆库**: `memorybank/`（6个核心文件）

### 9.3 外部参考
- **AI算法来源**: amazing-amazons项目的bot003
- **技术报告**: [bot003报告链接](https://github.com/RizzoHou/amazing-amazons/blob/main/docs/bots/reports/bot003_report_simplified_zh.md)

## 10. 总结

### 10.1 核心成就
1. **完整游戏实现**: 所有亚马逊游戏规则
2. **双界面支持**: 图形和文本模式
3. **AI系统**: 两种算法，外部集成
4. **增强功能**: 保存/加载、继续游戏、现代GUI
5. **全面测试**: 30个单元测试，质量保证

### 10.2 助教验收指引
1. **运行程序**: 按照第2节步骤构建和运行
2. **测试AI**: 第3节说明AI实现细节
3. **特色功能**: 第4节列出所有增强功能
4. **代码审查**: GitHub仓库包含完整源代码
5. **文档参考**: 所有技术细节在完整报告中

### 10.3 项目状态确认
- **基本功能**: 完全实现，可演示
- **AI能力**: 两种算法，文档齐全
- **特色功能**: 多项增强，用户友好
- **验收准备**: 已准备好第一次验收

---
**最后更新**: 2026年1月7日  
**项目状态**: 第3阶段完成，包含2026年1月更新，准备第2次验收  
**联系人**: GitHub Issues提交问题  
**保证**: 所有功能经过测试，文档准确完整
