#! /bin/python3
from os import path, listdir;
from sys import argv;
import subprocess;

def gen_for_xml(xml_path):
	print('generating code from \'' + xml_path + '\'')
	base_name = xml_path.rsplit('.', 1)[0] # strip .xml
	cmd = ['wayland-scanner', '--include-core-only']
	subprocess.call(cmd + ['server-header', xml_path, base_name + '.h'])
	subprocess.call(cmd + ['code', xml_path, base_name + '.c'])

def gen_for_all(dir_path):
	[ gen_for_xml(path.join(dir_path, xml)) for xml in listdir(dir_path) if xml.endswith('.xml') ]

def gen_needed(dir_path):
	return (
		len([path for path in listdir(dir_path) if path.endswith('.xml')]) ==
		len([path for path in listdir(dir_path) if path.endswith('.h')])
		)

if __name__ == '__main__':
	if len(argv) < 2:
		print('please specify the path for the protocols')
		exit(1)
	elif len(argv) > 2:
		print('too many arguments')
	dir_path = argv[1]
	gen_for_all(dir_path)