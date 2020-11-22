#!/usr/bin/python3.7
import subprocess
import pytest

def test_automata():
	executable_file = "./algo"
	test_folder = "./tests/test/"
	res_folder = "./tests/res/"
	list_file = ["hw4_1", "hw4_2", "test3"]
	for f in list_file:
		test_filename = "{}{}".format(test_folder, f)
		res_filename = "{}{}".format(res_folder, f)
		test_result = subprocess.check_output("{} {}".format(executable_file, test_filename), shell=True)
		test_result = str(test_result, 'utf-8')
		with open("{}_res".format(res_filename), 'r') as f:
			result = f.read()

			for res_line, test_line in zip(result.split('\n'), test_result.split('\n')):
				if res_line != test_line:
					print(test_filename, res_line)
					print(res_filename, test_line)

				assert (test_line == res_line)

if __name__ == "__main__":
	test_automata()