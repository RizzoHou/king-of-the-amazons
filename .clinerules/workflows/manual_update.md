# Game Manual Update Workflow

This workflow ensures the game manual (`docs/manuals/game_manual.md`) stays synchronized with the current project state. The manual serves as comprehensive documentation for users and should be updated after significant changes to the game.

## When to Update Game Manual

Trigger this workflow when:

1. **New Features Added**: New game modes, functionality, or commands
2. **UI Changes**: Menu system updates, display format changes
3. **Rule Changes**: Game mechanics or rule implementations modified
4. **Bug Fixes**: Significant fixes that affect user experience
5. **Phase Completion**: After completing major development phases
6. **User Requests**: When explicitly requested to update documentation
7. **Before Releases**: Prior to review sessions or project submissions

## Manual Structure Overview

Understanding the manual structure helps prioritize updates:

```
game_manual.md
├── Introduction (Project status, version)
├── Getting Started (Requirements, installation)
├── Game Rules (Board, movement, winning conditions)
├── User Interface Guide (Menus, display, input format)
├── Game Modes Detailed (Human vs Human, Human vs AI, AI vs AI)
├── Save/Load System (Usage, examples, file format)
├── Advanced Features (Undo, help, error handling)
├── Troubleshooting (Common issues, solutions)
├── Development Information (Project structure, testing)
├── Game Strategy Tips (Basic strategy, tactics)
├── Frequently Asked Questions
├── Version History (Phase completion tracking)
└── Credits & License
```

## Comprehensive Update Process

### Step 1: Review Current Project State

**BEFORE** updating the manual, understand what changed:

1. **Read Memory Bank Files** (Use existing workflow):
   - [ ] `memorybank/activeContext.md` - Recent changes and current work
   - [ ] `memorybank/progress.md` - What's working and what's complete
   - [ ] `memorybank/systemPatterns.md` - Architecture changes
   - [ ] `memorybank/techContext.md` - Technology updates

2. **Check Source Code Changes**:
   - [ ] Review recent git commits for feature additions
   - [ ] Check `src/ui/MenuController.cpp` for menu changes
   - [ ] Check `src/ai/` for AI algorithm updates
   - [ ] Check `src/utils/Serializer.cpp` for save/load changes

3. **Test Current Functionality**:
   - [ ] Run the game to verify current behavior
   - [ ] Test new features if applicable
   - [ ] Verify command syntax and error messages

### Step 2: Update Manual Sections (Priority Order)

Update sections in this order based on what changed:

#### High Priority: Version History & Introduction
**Update when**: Phase completion, major milestones
**Sections to update**:
- **Introduction → Current Status**: Update phase, version, completion percentage
- **Version History**: Add new phase completion entries
- **Getting Started**: Update if build/run instructions changed

**Example update**:
```markdown
### Current Status
- **Phase**: Phase 2 Complete (Complete Game System)
- **Version**: 1.0.0
- **Last Updated**: [Current Date]
- **Overall Completion**: 50% (Phase 1 & 2 complete, ready for Phase 3)
```

#### High Priority: Game Modes & Features
**Update when**: New game modes, AI improvements, feature additions
**Sections to update**:
- **Game Modes Detailed**: Add new modes, update AI descriptions
- **Advanced Features**: Document new commands or functionality
- **User Interface Guide**: Update menus, commands, display examples

**Example update**:
```markdown
### Human vs AI
**Description**: Play against computer opponent
**Player Roles**:
- **Human**: Plays as White (moves first)
- **AI**: Plays as Black ([New Algorithm Name])

**AI Characteristics**:
- **Algorithm**: [Updated algorithm description]
- **Strategy**: [New strategy details]
- **Difficulty**: [Updated difficulty level]
```

#### Medium Priority: Rules & Interface
**Update when**: Rule clarifications, input format changes
**Sections to update**:
- **Game Rules**: Clarify movement, winning conditions
- **User Interface Guide**: Update input format, command syntax
- **Save/Load System**: Document format changes, new features

#### Lower Priority: Reference & Support
**Update when**: Minor changes, bug fixes
**Sections to update**:
- **Troubleshooting**: Add new issues and solutions
- **Frequently Asked Questions**: Add new Q&A entries
- **Game Strategy Tips**: Add new tactical advice

### Step 3: Update Specific Sections (Detailed)

#### For New Game Modes
1. **Add to "Game Modes Detailed" section**
2. **Include**: Description, player roles, features, example gameplay
3. **Update "User Interface Guide"** with new menu options
4. **Add to "Version History"** as a feature addition

#### For AI Improvements
1. **Update AI descriptions** in relevant game modes
2. **Update "Advanced Features"** with algorithm details
3. **Consider adding "AI Strategy" subsection** if significant
4. **Update "Version History"** with AI enhancements

#### For UI/Menu Changes
1. **Update "User Interface Guide"** with new menu layouts
2. **Update screenshots/ASCII examples** (if applicable)
3. **Update "Getting Started"** if launch experience changed
4. **Update command tables** in "Advanced Features"

#### For Bug Fixes
1. **Update "Troubleshooting"** section
2. **Remove fixed issues** from common problems
3. **Update "Frequently Asked Questions"** if relevant
4. **Note in "Version History"** if significant fix

#### For Phase Completion
1. **Update "Introduction → Current Status"**
2. **Add new entry to "Version History"**
3. **Update completion percentages** throughout manual
4. **Review all sections** for phase-specific updates

### Step 4: Verify Manual Accuracy

**CRITICAL**: After updating, verify everything is accurate:

1. **Command Verification**:
   - [ ] All commands work as documented
   - [ ] Error messages match actual output
   - [ ] Input formats are correct

2. **Example Verification**:
   - [ ] Gameplay examples are valid
   - [ ] Save/load examples work
   - [ ] Menu navigation is accurate

3. **Build/Run Verification**:
   - [ ] Installation instructions work
   - [ ] Build commands are correct
   - [ ] System requirements are accurate

4. **Cross-Reference Check**:
   - [ ] Manual matches memory bank status
   - [ ] Version numbers are consistent
   - [ ] Feature lists are complete

### Step 5: Format and Polish

1. **Check Formatting**:
   - [ ] Markdown syntax is valid
   - [ ] Tables are properly formatted
   - [ ] Code blocks have correct language tags
   - [ ] Headings follow hierarchy

2. **Improve Readability**:
   - [ ] Complex concepts are explained clearly
   - [ ] Examples are helpful and relevant
   - [ ] Troubleshooting steps are actionable
   - [ ] Technical details are appropriate for audience

3. **Update Metadata**:
   - [ ] Update "Last Updated" date
   - [ ] Increment version if appropriate
   - [ ] Update completion percentages

## Update Guidelines

### Do ✅
- **Be Specific**: "Added Minimax algorithm with depth 3" not "Improved AI"
- **Include Examples**: Show actual commands and outputs
- **Update All Related Sections**: Don't just update one section
- **Test Everything**: Verify commands and examples work
- **Maintain Consistency**: Keep terminology consistent throughout

### Don't ❌
- **Document Unreleased Features**: Only document what's implemented
- **Include Implementation Details**: Focus on user perspective
- **Forget to Test**: Never document untested functionality
- **Create Contradictions**: Ensure manual doesn't contradict itself
- **Skip Verification**: Always verify after updating

## Special Cases

### Major Phase Completion (e.g., Phase 2 → Phase 3)
When completing a major development phase:

1. **Update Status Everywhere**:
   - Introduction status
   - Version history
   - Completion percentages

2. **Document New Phase Features**:
   - List all completed features
   - Document new functionality
   - Update game mode descriptions

3. **Update Future Plans**:
   - Update planned features for next phase
   - Adjust timeline if needed
   - Note any changes to project scope

### AI Algorithm Updates
When improving AI:

1. **Update Algorithm Description**:
   - Name and type of algorithm
   - Key characteristics
   - Performance expectations

2. **Update Game Mode Descriptions**:
   - AI difficulty levels
   - Strategy descriptions
   - Example gameplay updates

3. **Consider New Sections**:
   - AI strategy explanation (if educational focus)
   - Performance characteristics
   - Customization options (if any)

### User Interface Overhaul
When significantly changing UI:

1. **Complete Screenshot/Example Update**:
   - All ASCII examples
   - Menu layouts
   - Command prompts

2. **Update Navigation Instructions**:
   - Menu navigation paths
   - Command sequences
   - Error recovery steps

3. **Verify User Flow**:
   - Complete walkthroughs still work
   - All paths are documented
   - No dead ends in instructions

## Integration with Other Workflows

### Memory Bank Synchronization
This workflow should be executed **after** memory bank updates:
1. First: Update memory bank (`memory_bank_update.md` workflow)
2. Then: Update game manual (this workflow)
3. Finally: Update README if needed (`readme_update.md` workflow)

### Task Completion Integration
When completing tasks that affect documentation:
1. Complete the task implementation
2. Update memory bank
3. Update game manual (if needed)
4. Update README
5. Commit changes

### Version Control Integration
Manual updates should be committed with descriptive messages:
```bash
git add docs/manuals/game_manual.md
git commit -m "docs: update game manual for Phase 2 completion"
```

## Verification Checklist

Before completing manual update:

- [ ] All recent changes are documented
- [ ] Examples have been tested and work
- [ ] Commands produce expected results
- [ ] No contradictions with memory bank
- [ ] Version information is current
- [ ] Completion status is accurate
- [ ] Troubleshooting reflects current issues
- [ ] Formatting is consistent and clean
- [ ] All affected sections were updated
- [ ] Cross-references are correct

## Example Update Flow

```
Scenario: Just completed Phase 2 implementation

1. Review project state:
   - Read memory bank: Phase 2 complete in activeContext.md
   - Check source: New AI, save/load, enhanced menus
   - Test: Verify all Phase 2 features work

2. Update manual sections:
   - Introduction: Update status to "Phase 2 Complete"
   - Version History: Add Phase 2 completion entry
   - Game Modes: Add Human vs AI and AI vs AI descriptions
   - Save/Load System: Document JSON serialization
   - Advanced Features: Document new commands
   - Troubleshooting: Add Phase 2 specific issues

3. Verify accuracy:
   - Test all new commands
   - Verify examples work
   - Check consistency with memory bank

4. Format and polish:
   - Update "Last Updated" date
   - Ensure proper formatting
   - Verify readability

5. Commit changes:
   - git add docs/manuals/game_manual.md
   - git commit -m "docs: update manual for Phase 2 completion"
```

## Related Workflows

- **Memory Bank Update**: `.clinerules/workflows/memory_bank_update.md`
- **README Update**: `.clinerules/workflows/readme_update.md`
- **Task Completion**: `.clinerules/workflows/task_completion.md`

## Related Documentation

- **Game Manual**: `docs/manuals/game_manual.md`
- **Project Documentation**: `memorybank/` directory
- **Implementation Plans**: `docs/implementation/`

---

**Remember**: The game manual is the primary user-facing documentation. Keep it accurate, helpful, and synchronized with the actual implementation. Users rely on this document to understand and use the game effectively.
