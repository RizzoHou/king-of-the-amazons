# Implementation Plan Update Workflow

This workflow ensures the implementation plan documents in `docs/implementation/` accurately reflect the current project state and development approach. The implementation plan serves as the technical blueprint for the project and must stay synchronized with actual development progress, architectural decisions, and milestone achievements.

## When to Update Implementation Plan

Trigger this workflow when:

1. **Major milestones are reached** - Phase completion, review sessions passed, major features implemented
2. **Architectural changes occur** - Significant design pattern changes, technology stack updates
3. **Timeline adjustments needed** - Schedule changes, deadline shifts, resource reallocation
4. **Technical specifications evolve** - API changes, algorithm improvements, performance optimizations
5. **Testing strategy updates** - New testing approaches, coverage targets, automation improvements
6. **Risk landscape changes** - New risks identified, mitigation strategies adjusted
7. **Success criteria refinement** - Evaluation metrics updated, acceptance criteria clarified
8. **User explicitly requests** with **update implementation plan**

## Implementation Plan Files Overview

Understanding file relationships helps prioritize updates:

```
docs/implementation/
├── 01_project_structure.md      # File hierarchy and organization (rarely changes)
├── 02_implementation_phases.md  # Development timeline and milestones (frequent updates)
├── 03_technical_specifications.md # Technical details and APIs (medium frequency)
├── 04_testing_strategy.md       # Testing approach and coverage (medium frequency)
├── 05_risk_management.md        # Risk assessment and mitigation (medium frequency)
└── 06_success_criteria.md       # Success metrics and criteria (medium frequency)
```

## Comprehensive Update Process

### Step 1: READ ALL FILES (MANDATORY - NO EXCEPTIONS)

**CRITICAL RULE**: Before making ANY updates to the implementation plan, you MUST read EVERY relevant file using `read_file`. This is not optional.

**Required Reading Order** (use `read_file` on each):

#### Phase A: Read Memory Bank Files (Foundation Context)
1. [ ] `memorybank/projectbrief.md` - Core project goals and requirements
2. [ ] `memorybank/productContext.md` - User needs and problem statement
3. [ ] `memorybank/systemPatterns.md` - Architecture and design patterns
4. [ ] `memorybank/techContext.md` - Technologies and development setup
5. [ ] `memorybank/activeContext.md` - Current work and recent changes
6. [ ] `memorybank/progress.md` - Status tracking and accomplishments

#### Phase B: Read README.md (Project Overview)
7. [ ] `README.md` - Current project status and user-facing information

#### Phase C: Read Current Implementation Plan (Baseline)
8. [ ] `docs/implementation/01_project_structure.md`
9. [ ] `docs/implementation/02_implementation_phases.md`
10. [ ] `docs/implementation/03_technical_specifications.md`
11. [ ] `docs/implementation/04_testing_strategy.md`
12. [ ] `docs/implementation/05_risk_management.md`
13. [ ] `docs/implementation/06_success_criteria.md`

**Why ALL Files Matter**:
- Cannot determine what needs updating without understanding current project state
- Memory bank provides internal project context and recent developments
- README provides user-facing project status and overview
- Current implementation plan establishes baseline for comparison
- Ensures updates maintain consistency across all documentation
- Prevents contradictions between different documentation sources

**REMEMBER**: You must read ALL 13 files above before making any updates. Skipping files = incomplete understanding = poor updates.

### Step 2: Analyze and Identify Updates Needed

After reading all files, analyze what needs updating:

#### High Priority Analysis: Phase Completion and Timeline
- **Check**: Does `02_implementation_phases.md` reflect actual phase completion status?
- **Compare**: Phase completion in `02_implementation_phases.md` vs `progress.md`
- **Identify**: Timeline adjustments needed based on actual progress
- **Document**: New milestones achieved or schedule changes

#### Medium Priority Analysis: Technical and Strategic Updates
- **Technical Specifications**: Does `03_technical_specifications.md` match actual implementation?
- **Testing Strategy**: Does `04_testing_strategy.md` reflect current testing approach?
- **Risk Management**: Does `05_risk_management.md` include current risks and mitigations?
- **Success Criteria**: Does `06_success_criteria.md` align with current evaluation metrics?

#### Low Priority Analysis: Structural Updates
- **Project Structure**: Does `01_project_structure.md` match actual file organization?
- **Cross-Document Consistency**: Are there contradictions between implementation plan files?

### Step 3: Update Implementation Plan Files (Priority Order)

**ONLY AFTER completing Steps 1 and 2**, update implementation plan files in this priority order:

#### High Priority: `02_implementation_phases.md`
Update with:
- **Phase completion status**: Mark completed phases with ✅, update percentages
- **Timeline adjustments**: Reflect actual schedule vs planned schedule
- **Milestone achievements**: Document completed milestones and deliverables
- **Schedule updates**: Adjust future timelines based on current progress
- **Success criteria met**: Update what's working based on `progress.md`

#### Medium Priority: `05_risk_management.md` and `06_success_criteria.md`
Update `05_risk_management.md` with:
- **New risks identified**: From recent development experience
- **Risk status changes**: Risks that materialized or were mitigated
- **Updated mitigation strategies**: Based on what worked/didn't work
- **Current risk assessment**: Severity and probability adjustments

Update `06_success_criteria.md` with:
- **Achieved success criteria**: Mark completed evaluation metrics
- **Updated targets**: Adjust performance targets based on experience
- **New criteria**: Add criteria for newly implemented features
- **Grading alignment**: Ensure criteria match academic requirements

#### Medium Priority: `03_technical_specifications.md` and `04_testing_strategy.md`
Update `03_technical_specifications.md` with:
- **Architecture changes**: Updated component relationships
- **API updates**: Modified interfaces or new APIs
- **Algorithm improvements**: Enhanced implementations
- **Performance characteristics**: Actual vs planned performance

Update `04_testing_strategy.md` with:
- **Testing coverage**: Actual test coverage achieved
- **New test approaches**: Testing methods implemented
- **Automation status**: Current test automation level
- **Quality metrics**: Actual bug rates and quality measures

#### Low Priority: `01_project_structure.md`
Update with:
- **File organization changes**: Added/removed directories or files
- **Build system updates**: CMake/Makefile changes
- **Toolchain adjustments**: Development tool updates
- **Dependency changes**: New or removed external libraries

### Step 4: Ensure Consistency

Cross-check files for alignment:

- [ ] `02_implementation_phases.md` phase completion matches `progress.md` status
- [ ] `03_technical_specifications.md` matches actual code architecture
- [ ] `04_testing_strategy.md` aligns with actual test suite
- [ ] `05_risk_management.md` reflects current project challenges
- [ ] `06_success_criteria.md` evaluation metrics are current
- [ ] All implementation plan files tell a coherent story
- [ ] No contradictions between implementation plan and memory bank

### Step 5: Document Update Rationale

Capture key decisions and changes:
- **Why updates were made**: "Updated Phase 3 timeline because graphical GUI implementation took longer than expected"
- **What changed from original plan**: "Changed from Minimax AI to Greedy AI due to time constraints"
- **Lessons learned**: "Found that modular architecture worked better than monolithic approach"
- **Future implications**: "Phase 4 timeline adjusted to account for enhanced features complexity"

## Update Guidelines

### Do ✅
- **Be specific**: "Phase 2 completed Dec 30 with all basic functionality working" not "Made progress"
- **Reference actual code**: "Implemented BasicAI with mobility heuristic" not "Added AI"
- **Maintain timeline accuracy**: Update dates and percentages based on actual progress
- **Document deviations**: Explain why plans changed from original implementation plan
- **Keep technical accuracy**: Ensure specifications match actual implementation

### Don't ❌
- **Speculate about future**: Only document what's actually implemented or decided
- **Duplicate memory bank content**: Focus on implementation plan perspective
- **Include temporary workarounds**: Document final solutions, not interim fixes
- **Overcomplicate timelines**: Keep schedules clear and achievable
- **Forget to verify**: Always check against actual code and memory bank

## Special Case: User Requests "Update Implementation Plan"

When user explicitly requests an implementation plan update, follow this MANDATORY sequence:

### Phase 1: READ EVERYTHING (No Updates Yet)

**YOU MUST USE `read_file` ON EACH FILE INDIVIDUALLY in exact order:**

```
1. memorybank/projectbrief.md
2. memorybank/productContext.md
3. memorybank/systemPatterns.md
4. memorybank/techContext.md
5. memorybank/activeContext.md
6. memorybank/progress.md
7. README.md
8. docs/implementation/01_project_structure.md
9. docs/implementation/02_implementation_phases.md
10. docs/implementation/03_technical_specifications.md
11. docs/implementation/04_testing_strategy.md
12. docs/implementation/05_risk_management.md
13. docs/implementation/06_success_criteria.md
```

**DO NOT SKIP ANY FILES** - You cannot know what needs updating without reading everything first.

**DO NOT START UPDATING** until you have read all 13 files.

### Phase 2: ANALYZE (After Reading All Files)

Now that you've read everything, identify:

- **Phase completion discrepancies**: What's documented vs what's actually completed
- **Timeline misalignments**: Planned vs actual schedule
- **Technical specification gaps**: Documented vs implemented features
- **Testing strategy updates**: Current testing approach vs documented strategy
- **Risk assessment changes**: New risks or mitigated risks not documented
- **Success criteria evolution**: Achieved criteria vs documented targets

### Phase 3: UPDATE (Make Changes)

Only after reading all files and analyzing, update in priority order:

1. **First**: Update `02_implementation_phases.md` with current phase status
2. **Second**: Update `05_risk_management.md` and `06_success_criteria.md`
3. **Third**: Update `03_technical_specifications.md` and `04_testing_strategy.md`
4. **Fourth**: Update `01_project_structure.md` if needed

### Phase 4: VERIFY

Ensure:
- [ ] All 13 required files were read before any updates
- [ ] Updates maintain consistency across implementation plan
- [ ] Implementation plan aligns with memory bank and README
- [ ] Technical specifications match actual implementation
- [ ] Nothing important was missed

## Common Update Scenarios

### Scenario 1: Phase Completion
```
Trigger: Major phase completed (e.g., Phase 2: Complete Game)
Update Priority:
  1. 02_implementation_phases.md: Mark phase as complete, update timeline
  2. 06_success_criteria.md: Update achieved criteria for completed phase
  3. 05_risk_management.md: Document risks that materialized/were mitigated
  4. 03_technical_specifications.md: Update with implemented features
  5. 04_testing_strategy.md: Document testing completed for phase
```

### Scenario 2: Architectural Change
```
Trigger: Significant architecture change (e.g., added graphical interface)
Update Priority:
  1. 03_technical_specifications.md: Update architecture and APIs
  2. 02_implementation_phases.md: Adjust timeline for architectural work
  3. 05_risk_management.md: Add/update risks related to architecture change
  4. 04_testing_strategy.md: Update testing approach for new architecture
  5. 01_project_structure.md: Update file organization if changed
```

### Scenario 3: Timeline Adjustment
```
Trigger: Schedule changes due to delays or acceleration
Update Priority:
  1. 02_implementation_phases.md: Update all affected timelines
  2. 05_risk_management.md: Add schedule-related risks
  3. 06_success_criteria.md: Adjust evaluation timeline if needed
  4. Other files: Update as needed based on schedule impact
```

### Scenario 4: Technical Specification Update
```
Trigger: API changes, algorithm improvements, performance updates
Update Priority:
  1. 03_technical_specifications.md: Update technical details
  2. 04_testing_strategy.md: Update tests for changed specifications
  3. 02_implementation_phases.md: Note specification changes in relevant phase
  4. 06_success_criteria.md: Update performance criteria if changed
```

## Verification Checklist

Before completing the update:

- [ ] All 13 required files were read before any updates
- [ ] Phase completion status is accurate and matches `progress.md`
- [ ] Technical specifications match actual implementation
- [ ] Testing strategy reflects current approach
- [ ] Risk assessment includes current project challenges
- [ ] Success criteria are achievable and current
- [ ] Project structure matches actual file organization
- [ ] No contradictions between implementation plan files
- [ ] Implementation plan aligns with memory bank
- [ ] Timeline is realistic based on current progress

## Implementation Plan Philosophy

Remember: **The implementation plan is the technical blueprint that guides development.**

- Write for "future developers" who need to understand the technical approach
- Prioritize accuracy over optimism - document what actually happened
- Maintain clear connection between plans and actual implementation
- Update when reality diverges from plans (which is expected in development)
- Keep it actionable for guiding future development work

## Example Update Flow

```
Scenario: Just completed Phase 2 implementation

1. Read all 13 required files (memory bank, README, implementation plan)
2. Analyze:
   - Phase 2 marked complete in progress.md
   - Basic AI, save/load, enhanced menus implemented
   - Timeline: Completed Dec 30 (on schedule)
   - Risks: AI performance risk mitigated with greedy algorithm
   
3. Update implementation plan:
   - 02_implementation_phases.md: Mark Phase 2 as 100% complete
   - 06_success_criteria.md: Update achieved criteria for Phase 2
   - 05_risk_management.md: Document AI performance risk mitigation
   - 03_technical_specifications.md: Add BasicAI and Serializer details
   - 04_testing_strategy.md: Document Phase 2 testing completed
   
4. Verify consistency:
   - Phase completion matches progress.md ✓
   - Technical specs match implemented code ✓
   - All files tell coherent story ✓
```

## Integration with Other Workflows

### Relationship with Memory Bank Update
- **Memory Bank**: Internal project context, recent work, current focus
- **Implementation Plan**: Technical blueprint, development approach, timelines
- **Update Order**: Typically update memory bank first, then implementation plan

### Relationship with README Update
- **README**: User-facing overview, setup instructions, current status
- **Implementation Plan**: Technical details, development strategy, internal planning
- **Consistency**: Both should align on project status and milestones

### Standalone Workflow
This workflow is designed as a **standalone process** triggered by:
- Major milestone completion
- Significant architectural changes
- User explicit request
- Regular milestone-based reviews

It is **NOT integrated into task_completion.md** to avoid overcomplicating routine task completion.

---

**Related Documentation**: `docs/implementation/` directory
**Related Rules**: `.clinerules/documentation.md`
**Workflow Reference**: Similar patterns to `memory_bank_update.md`, `readme_update.md`

*Last Updated: 2025-12-21*
