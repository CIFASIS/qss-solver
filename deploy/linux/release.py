#!/usr/bin/env python3

import os
import sys
import subprocess
import argparse
from pathlib import Path

def run_command(cmd, check=True, capture_output=False):
    """Run a shell command and handle errors"""
    print(f"Running: {cmd}")
    try:
        if capture_output:
            result = subprocess.run(cmd, shell=True, check=check, 
                                  capture_output=True, text=True)
            return result.stdout.strip(), result.stderr.strip()
        else:
            subprocess.run(cmd, shell=True, check=check)
            return None, None
    except subprocess.CalledProcessError as e:
        print(f"Error: {e}")
        if capture_output:
            return None, e.stderr
        sys.exit(1)

def get_current_version():
    """Get current version from version file"""
    try:
        with open("version", "r") as f:
            return f.read().strip()
    except FileNotFoundError:
        print("Error: version file not found")
        sys.exit(1)

def update_version_file(new_version):
    """Update version file with new version"""
    print(f"Updating version file to {new_version}")
    with open("version", "w") as f:
        f.write(new_version)

def check_git_status():
    """Check if git working directory is clean"""
    stdout, stderr = run_command("git status --porcelain", capture_output=True)
    if stdout:
        print("Error: Git working directory is not clean. Please commit or stash changes first.")
        print("Uncommitted files:")
        print(stdout)
        sys.exit(1)

def main():
    parser = argparse.ArgumentParser(description="CIFASIS M&S Repositories Release Script")
    parser.add_argument("version", help="New version number (e.g., 1.2.3)")
    parser.add_argument("--repo", default="qss-solver", 
                       help="Repository name (default: qss-solver)")
    args = parser.parse_args()

    # Validate version format
    version_parts = args.version.split(".")
    if len(version_parts) != 3:
        print("Error: Version must be in format X.X.X (e.g., 1.2.3)")
        sys.exit(1)

    new_version = args.version
    repo_name = args.repo
    release_branch = f"release-{new_version}"

    print(f"Starting release process for version {new_version} in repository {repo_name}")
    print("=" * 50)

    # Step 1: Check git status
    print("Step 1: Checking git status...")
    #check_git_status()

    # Step 2: Checkout main release branch
    print(f"Step 2: Checking out {repo_name}-release branch...")
    run_command(f"git checkout {repo_name}-release")

    # Step 3: Create release branch
    print(f"Step 3: Creating branch {release_branch}...")
    run_command(f"git checkout -b {release_branch}")

    # Step 4: Merge main dev branch -> release branch
    print(f"Step 4: Merging {repo_name}-dev into release-{new-version}...")
    run_command(f"git merge {repo_name}-dev")

    # Step 5: Update version file
    print(f"Step 5: Updating version file to {new_version}...")
    update_version_file(new_version)

    # Step 6: Run update.sh
    print("Step 6: Running update-version.sh...")
    run_command("./update-version.sh")

    # Step 7: Commit update changes
    print("Step 7: Committing update changes...")
    run_command("git add version")
    run_command(f"git commit -m \"Update version to {new_version}\"")

    # Step 8: Update CHANGELOG and README.md files
    print("Step 8: Updating CHANGELOG and README.md files...")
    # This step might need manual intervention or specific commands
    print("Note: Please manually update CHANGELOG and README.md files with release notes")
    response = input("Have you updated CHANGELOG and README.md? (y/n): ")
    if response.lower() != 'y':
        print("Release aborted. Please update CHANGELOG and README.md files first.")
        sys.exit(1)

    run_command("git add CHANGELOG README.md")
    run_command(f"git commit -m \"Update CHANGELOG and README.md for version {new_version}\"")

    # Step 9: Push branch
    print("Step 9: Pushing branch...")
    run_command(f"git push origin {release_branch}")

    # Step 10: Merge pushed branch in release branch with --squash
    print(f"Step 10: Merging pushed branch in {repo_name}-release with --squash...")
    run_command(f"git checkout {repo_name}-release")
    run_command(f"git merge {release_branch} --squash")
    run_command(f"git commit")
    run_command(f"git push origin {repo_name}-release")

    # Step 11: Push release branch
    print(f"Step 11: Pushing {repo_name}-release branch...")
    run_command(f"git push origin {repo_name}-release")

    # Step 12: Merge release branch in dev branch
    print(f"Step 12: Merging {repo_name}-release in {repo_name}-dev...")
    run_command(f"git checkout {repo_name}-dev")
    run_command(f"git merge {repo_name}-release")

    print("=" * 50)
    print(f"Release {new_version} completed successfully!")
    print(f"Don't forget to:")
    print("1. Create a tag: git tag -a v{new_version} -m 'Version {new_version}'")
    print("2. Push tag: git push origin v{new_version}")
    print("3. Create GitHub release if needed")

if __name__ == "__main__":
    main()
