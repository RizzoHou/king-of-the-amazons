NOTE: all discussions are in the context of GUI mode, not the TEXT mode.

**PROBLEM**: The mode of ai vs ai is both unusable and unnecessary. It’s unusable because it can cause the game to stuck at the first turn. It’s unnecessary because I don’t need such a mode.

**STATUS**: RESOLVED (2025-12-23)

**SOLUTION**: AI vs AI mode has been completely removed from the graphical interface. The graphical mode selection screen now only shows:
1. Continue Previous Game (when available)
2. Human vs Human
3. Human vs AI

**RATIONALE**:
1. The user explicitly stated "I don't need such a mode"
2. AI vs AI mode was causing the game to get stuck at the first turn in the graphical interface
3. The mode is not required by the assignment specifications
4. Text mode (`--text` flag) still supports AI vs AI for testing/demonstration purposes

**CHANGES MADE**:
1. Removed AI vs AI button from the graphical mode selection screen
2. Updated button layout and positioning
3. Removed all AI vs AI related logic from `GraphicalController.cpp`
4. Updated comments to reflect that AI vs AI mode has been removed

**NOTE**: The text mode interface (`MenuController.cpp`) still supports AI vs AI mode for those who want to use it with the `--text` flag.
