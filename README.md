# RTL Migration Witness — Altera-inspired contract experiment

Altera develops FPGAs and Quartus tooling. Its documentation describes IP migration and generated simulation files. Migration contracts are the motivation for this independent experiment; no company affiliation, missing product claim or roadmap knowledge is implied.

**Implemented scope: bounded C++ behavioral exploration, not RTL simulation or formal equivalence.** This repository does not parse or execute Verilog/SystemVerilog. It enumerates explicitly authored mathematical transition functions and emits replayable shortest counterexamples. The two SystemVerilog modules are human-authored illustrations only; correspondence between them and the C++ models is unverified. No vendor IP, FIFO, handshake adapter, Quartus integration, timing, board or hardware result is included.

```sh
make test
make demo
./rtl-witness > report.json
make tools
```

Requirements: g++ with C++20 and Make, no dependencies, account or hardware. `demo.json` is an actual local run. A three-bit wrapping counter disagrees with saturation after eight enabled clocks; changed reset polarity and ignored enable disagree after one. Identical models yield `no_witness_within_bound`, not a hardware equivalence certificate. A separate exhaustive stateless check enumerates all eight three-bit inputs for three-to-six-bit signed versus zero extension; input four becomes 60 versus four in unsigned bit-pattern notation.

## Contract and search

One synchronous clock domain, active-high reset takes precedence over enable, output is the register value immediately after each modeled clock, and initial state is explicitly zero. RTL hardware generally needs a reset to establish that state: zero initialization is a C++ environment assumption, not an inferred SV power-up property. Counter width is 1–8; depth bound is 0–32. All four Boolean reset/enable combinations are considered at each reachable paired state. Breadth-first exploration memoizes paired states, so a witness is shortest in clocks for these deterministic finite-state transition functions. Results record bounds and examined transitions. Lack of witness has only the documented model meaning. The signed-extension checker allows input widths 1–8, output widths input–16, uses unsigned masks, and rejects impossible widths/values.

The committed tests check exact overflow witness lengths for widths 1–5, witness replay and tampering, shorter bounds that deliberately miss a mutation, equal-model controls, reset and enable priority, signed-extension bit patterns across all supported widths, and invalid assumptions. Broader widths can place overflow outside the 32-cycle search bound and must not be called equivalent.

## Tool availability and next integration

On this environment, `command -v yosys`, `verilator`, and `iverilog` returned no executable; `apt-cache policy` returned no package candidates. Official installation options were checked (links below). HDL tooling was not downloaded or installed. This scoped fallback avoids treating behavioral C++ as an HDL verifier. `make tools` repeats executable discovery on your machine; it does not integrate a newly installed tool. Next stage: install an official Verilator/Yosys release, run the SV fixtures with an explicit reset harness, verify model correspondence, then add a Quartus-generated model adapter and independent RTL traces. Those stages are unverified.

Primary sources for company relevance and existing tools:

- https://docs.altera.com/r/docs/683463/25.3/quartus-prime-pro-edition-user-guide/migrating-ip-cores-to-a-different-device
- https://docs.altera.com/r/docs/683463/25.3/quartus-prime-pro-edition-user-guide/generating-ip-simulation-files
- https://yosyshq.readthedocs.io/projects/yosys/en/docs-preview-contributing_help/cmd/index_passes_equiv.html
- https://yosysHQ.net/yosys/download.html
- https://verilator.org/guide/latest/install.html

Yosys and Quartus already offer substantial verification/migration capabilities. Our inferred opportunity is interpretable migration-contract evidence around those tools, not a new formal engine.
