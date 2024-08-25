# Compare Packages Utility

## Overview

This utility compares binary packages from two branches of the ALT Linux package repository and generates a JSON file showing:
- Packages present in branch 1 but not in branch 2.
- Packages present in branch 2 but not in branch 1.
- Packages with higher version-release in branch 1 than in branch 2.
- When you run the utility, a json file with the comparison result will be saved

## Requirements

- ALT Linux Workstation K 10.2
- Development tools (g++, make)
- libcurl
- nlohmann/json (for JSON manipulation)

## Installation

1. **Clone the repository:**

   ```bash
   git clone https://github.com/orange3107/baseAlt_test.git

2. **Build and Install:**

   ```bash
   cd project_root
   make
   sudo make install
   sudo ldconfig

## Usage

**Run the utility**

   ```bash
   compare_packages
