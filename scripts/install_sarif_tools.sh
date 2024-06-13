#!/usr/bin/env bash
sudo curl -sSL https://github.com/psastras/sarif-rs/releases/download/clang-tidy-sarif-latest/clang-tidy-sarif-x86_64-unknown-linux-gnu -o /usr/local/bin/clang-tidy-sarif
sudo chmod +x /usr/local/bin/clang-tidy-sarif

sudo curl -sSL https://github.com/psastras/sarif-rs/releases/download/sarif-fmt-latest/sarif-fmt-x86_64-unknown-linux-gnu -o /usr/local/bin/sarif-fmt
sudo chmod +x /usr/local/bin/sarif-fmt

dotnet tool install --global Sarif.Multitool
