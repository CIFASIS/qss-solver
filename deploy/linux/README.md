**Release procedure**

- Checkout `qss-solver-release` branch.
- Create branch `release-X.X.X`.
- Merge `qss-solver-dev` -> `qss-solver-release`.
- Update `version` file to `X.X.X`.
- Run `update.sh`.
- Commit update changes with `-n`.
- Update `CHANGELOG` and `README.md` files.
- Commit changes with `-n`.
- Push branch.
- Merge pushed branch in `qss-solver-release` with `--squash`.
- Push `qss-solver-release` branch.
- Merge `qss-solver-release` in `qss-solver-dev`.