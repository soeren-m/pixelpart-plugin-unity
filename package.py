import sys
import os
import os.path
import argparse
import tarfile

parser = argparse.ArgumentParser(description='Create Unity package')
parser.add_argument('-o', '--output', required=True, help='Output path')
parser.add_argument('-t', '--target', required=True, help='Target directory')

args = parser.parse_args()

print('Target directory:', args.target)
print('Output path:', args.output)

if not os.path.exists(args.target):
    print('Target does not exist:', args.target)
    sys.exit(1)

with tarfile.open(args.output, 'w:gz') as tar:
    tar.add(args.target, arcname="package")
