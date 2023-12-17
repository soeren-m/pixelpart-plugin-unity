import sys
import os
import os.path
import io
import argparse
import tarfile
import yaml
import glob

parser = argparse.ArgumentParser(description='Create unitypackage')
parser.add_argument('-o', '--output', required=True, help='Output path')
parser.add_argument('targets', nargs='*', help='Target directory or file')

args = parser.parse_args()

print('Target directory or file:', args.targets)
print('Output path:', args.output)

for target in args.targets:
    if not os.path.exists(target):
        print('Target does not exist:', target)
        sys.exit(1)

def filter_file(tinfo):
    tinfo.uid = tinfo.gid = 0
    tinfo.uname = tinfo.gname = "root"
    tinfo.mode = 0o777

    return tinfo

def add_file(tar, metapath):
    filepath = metapath[0:-5].replace('\\', '/')
    print(f'Adding file "{filepath}"')

    with open(metapath, 'r') as metafile:
        try:
            guid = yaml.safe_load(metafile)['guid']
        except yaml.YAMLError as ex:
            print(ex)
            return

    tarinfo = tarfile.TarInfo(guid)
    tarinfo.type = tarfile.DIRTYPE
    tarinfo.mode = 0o777
    tar.addfile(tarinfo=tarinfo)

    if os.path.isfile(filepath):
        tar.add(filepath, arcname=os.path.join(guid, 'asset'), filter=filter_file)
    tar.add(metapath, arcname=os.path.join(guid, 'asset.meta'), filter=filter_file)

    tarinfo = tarfile.TarInfo(os.path.join(guid, 'pathname'))
    tarinfo.mode = 0o777
    tarinfo.size = len(filepath)
    tar.addfile(tarinfo=tarinfo, fileobj=io.BytesIO(filepath.encode('utf8')))

with tarfile.open(args.output, 'w:gz') as tar:
    for target in args.targets:
        add_file(tar, target + '.meta')

        for meta in glob.glob(os.path.join(target, '**/*.meta'), recursive=True):
            add_file(tar, meta)