#!/usr/bin/python

import argparse
import subprocess
import os
import shutil
import sys
    
def main():
    
    parser = argparse.ArgumentParser("update")
    parser.add_argument("--branch_name", help="Checkout branch name.", type=str, default='sb-graph-dev')
    parser.add_argument("--repo_checkout", help="Clone method for the repo, it could be: https or ssh.", type=str, default='https')
    args = parser.parse_args()
    branch_name = args.branch_name
    repo_url = 'git@github.com:CIFASIS/sb-graph.git'
    if args.repo_checkout == 'https':
        repo_url = 'https://github.com/CIFASIS/sb-graph.git'
    if os.path.isdir('./sb-graph'):
        shutil.rmtree('./sb-graph')
    subprocess.check_call(['git', 'clone', '-b' , branch_name, '--single-branch', repo_url])
    shutil.rmtree('./sb-graph-dev', True)
    shutil.move('./sb-graph', './sb-graph-dev')
    subprocess.check_call(['tar', '-czvf', 'sb-graph-dev.tar.gz', './sb-graph-dev'])
    shutil.rmtree('./sb-graph-dev')

if (__name__ == '__main__'):
    main()

