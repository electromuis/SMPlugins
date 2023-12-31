import argparse
import os
import configparser
import zipfile
import io
import requests
import shutil

current_dir = os.path.dirname(os.path.realpath(__file__))
skds_dir = os.path.join(current_dir, 'sdks')
if not os.path.exists(skds_dir):
    os.makedirs(skds_dir)

plugins_dir = os.path.join(current_dir, 'plugins')
if not os.path.exists(plugins_dir):
    os.makedirs(plugins_dir)

tmp_dir = os.path.join(current_dir, 'tmp')
if not os.path.exists(tmp_dir):
    os.makedirs(tmp_dir)

built_dir = os.path.join(current_dir, 'built')
if not os.path.exists(built_dir):
    os.makedirs(built_dir)

def get_sdk(name):
    version = 'latest'

    pts = name.split('-')
    if len(pts) == 3:
        author, name, version = name.split('-')
    if len(pts) == 2:
        author, name = name.split('-')
    if len(pts) == 1:
        author = name
        name = name

    tags = requests.get('https://api.github.com/repos/{}/{}/tags'.format(author, name)).json()
    if version == 'latest':
        version = tags[0]['name']

    sdk_path = None
    for sdk in os.listdir(skds_dir):
        if sdk.endswith('-sdk'):
            sdk_author, sdk_name, sdk_version, pfix = sdk.split('-')
            if sdk_name == name and (version == 'latest' or sdk_version == version):
                sdk_path = os.path.join(skds_dir, sdk)
                break

    if sdk_path is None:
        sdk_tmp = os.path.join(tmp_dir, 'sdk.zip')
        if os.path.exists(sdk_tmp):
            os.unlink(sdk_tmp)
        
        with open(sdk_tmp, 'wb') as f:
            print('Downloading ...')
            sdk_url = 'https://github.com/{}/{}/releases/download/{}/SDK.zip'.format(author, name, version)
            f.write(requests.get(sdk_url).content)

        print('Unpacking ...')
        sdk_path = os.path.join(skds_dir, '{}-{}-{}-sdk'.format(author, name, version))
        ziphandle = zipfile.ZipFile(sdk_tmp)
        ziphandle.extractall(sdk_path)

    return sdk_path

def download_plugin(args):
    pts = args.url.split('/')
    name = pts[-1]
    plugin_path = os.path.join(plugins_dir, name)
    
    if os.path.exists(plugin_path):
        print("Plugin already exists: %s" % plugin_path)
        return
    
    os.system('git clone %s %s' % (args.url, plugin_path))
    os.chdir(plugin_path)
    os.system('git submodule update --init --recursive')

    print("Plugin downloaded: %s" % plugin_path)

def plugin_build(args):
    plugin_path = os.path.join(plugins_dir, args.plugin)
    if not os.path.exists(plugin_path):
        print("Plugin not exists: %s" % plugin_path)
        return
    
    # sdk_path = os.path.join(skds_dir, args.sdk)
    sdk_path = get_sdk(args.sdk)
    if not os.path.exists(sdk_path):
        print("SDK not exists: %s" % sdk_path)
        return
    
    tmp_build_dir = os.path.join(tmp_dir, 'build')
    if os.path.exists(tmp_build_dir):
        shutil.rmtree(tmp_build_dir)

    os.mkdir(tmp_build_dir)
    os.chdir(tmp_build_dir)

    command = [
        'cmake',
        '-DCMAKE_MODULE_PATH="%s"' % sdk_path.replace('\\', '/'),
        # '-DPLUGIN_OUT_DIR="%s"' % built_dir.replace('\\', '/'),
        '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY="%s"' % built_dir.replace('\\', '/'),
        '-DCMAKE_RUNTIME_OUTPUT_DIRECTORY="%s"' % built_dir.replace('\\', '/'),
        '-DCMAKE_RUNTIME_OUTPUT_DIRECTORY_{}="{}"'.format(args.config.upper(), built_dir.replace('\\', '/')),
        '-DCMAKE_BUILD_TYPE={}'.format(args.config)
    ]

    if args.compiler == 'msvc':
        command = [
            'PATH=/opt/msvc/bin/x64:$PATH',
            'CXX=cl',
            'CC=cl',
        ] + command + [
            '-DCMAKE_SYSTEM_NAME=Windows'
        ]

    print("Building ...", command)

    os.system(' '.join(command + [plugin_path]))
    os.system('cmake --build . --config {}'.format(args.config))

    # if args.compiler == 'msvc':
    #     # copy dll
    #     dll_path = os.path.join(tmp_build_dir, '%s.dll' % args.plugin)
    #     shutil.copy(dll_path, built_dir)

def sdk_list():
    for name in os.listdir(skds_dir):
        parser = configparser.ConfigParser()
        parser.read(os.path.join(skds_dir, name, 'version.ini'))
        
        print("Path: ", os.path.join(skds_dir, name))
        print("Name: ", parser.get('main', 'name'))
        print("Version: ", parser.get('main', 'version'))

def plugin_list():
    for name in os.listdir(plugins_dir):
        print("Path: ", os.path.join(plugins_dir, name))
        print("Name: ", name)


parser = argparse.ArgumentParser()
subparsers = parser.add_subparsers(dest='command')

sdk_download_parser = subparsers.add_parser('sdk_download')
sdk_download_parser.add_argument('--url', required=True)

plugin_download_parser = subparsers.add_parser('plugin_download')
plugin_download_parser.add_argument('--url', required=True)

plugin_build_parser = subparsers.add_parser('plugin_build')
plugin_build_parser.add_argument('--plugin', required=True)
plugin_build_parser.add_argument('--sdk', required=True)
plugin_build_parser.add_argument('--compiler')
plugin_build_parser.add_argument('--config', default='Release')

list_sdks_parser = subparsers.add_parser('sdk_list')
list_plugins_parser = subparsers.add_parser('plugin_list')

args = parser.parse_args()
if args.command == 'plugin_download':
    download_plugin(args)
elif args.command == 'plugin_build':
    plugin_build(args)
elif args.command == 'sdk_list':
    sdk_list()
elif args.command == 'plugin_list':
    plugin_list()
else:
    parser.print_help()
