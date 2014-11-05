#!/usr/bin/env python2.6
# -*- coding: utf-8 -*-u
## asgeirom ifi uio no
## Test for the string assignment (1.) INF1060 2013
## On IFI machines "python2.6 string_test.py ./my_string_program"
import re
import subprocess

from sys import stdin, stdout, stderr
from time import sleep
from os.path import isfile
from threading import Thread

test_strings = [
	u"Tre små kinesere på Høybroplass\n"
		u"satt og spilte på en kontrabass.\n"
		u"Så kom en konstabel, spurte hva var hendt,\n"
		u"tre små kinesere på Høybroplass.",
	u"En bussjåfør en bussjåfør\n"
		u"det er en mann med godt humør\n"
		u"Og har han ikke godt humør\n"
		u"da er han ingen bussjåfør\n"
		u"En bussjåfør en bussjåfør\n"
		u"det er en mann med godt humør.\n",
	]

test_encodings = [u"LATIN1", u"UTF8", ]

vokals_eng = [u"a", u"e", u"i", u"o", u"u", u"y", ]

vokals_nor = [u"æ", u"ø", u"å", ]

vokals_tot = vokals_eng + vokals_nor

vokals_meng = re.compile(u'([%s])' % u"".join(vokals_eng), re.UNICODE | re.IGNORECASE)
vokals_mnor = re.compile(u'([%s])' % u"".join(vokals_nor), re.UNICODE | re.IGNORECASE)
vokals_match = re.compile(u'([%s])' % u"".join(vokals_tot), re.UNICODE | re.IGNORECASE)

# <-- Start super bad -->
# If you program like this in a graded delivery,
# exspect negative points for bad flow.
def_program = u"./string"
if __name__ == '__main__':
	from sys import argv, exit
	if len(argv) > 1 and isfile(argv[1]):
		def_program = argv[1]
	elif len(argv) > 1 and not isfile(argv[1]):
		stderr.write(u"Argument is not a file!\n\t%s\n" % argv[1])
		exit(-1)
	else:
		stderr.write(u"No argument give usage:\n\t%s <./your_string_prog>\n" % argv[0])
		exit(0)
# <-- End super bad -->

def string_program(program = def_program, argument = u"print"):
	return subprocess.Popen(
		[program, argument],
		stdin = subprocess.PIPE,
		stdout = subprocess.PIPE)

class Feeder(Thread):
	def __init__(self, *args, **kwargs):
		self._text = kwargs.pop('text', u"")
		self._program = kwargs.pop('program', None)
		self._encoding = kwargs.pop('encoding', u"LATIN1")
		self._result = None
		return super(Feeder, self).__init__(*args, **kwargs)
	def run(self):
		if not self._program:
			self._result = False
			return
		if not hasattr(self._program, 'stdin'):
			self._result = False
			return
		p_stdin = self._program.stdin
		if not hasattr(p_stdin, 'write'):
			self._result = False
			return
		if not hasattr(self._text, 'encode'):
			stderr.write(u"Text did not have encode option!\n")
			p_stdin.write(self._text.encode)
		else:
			p_stdin.write(self._text.encode(self._encoding))
		self._result = True
		p_stdin.close()
		return

def print_test(string, debug=True):
	if debug: stderr.write(u"#### Print line test:\n")
	find = []
	for line in string.split('\n'):
		line = line.strip()
		if not line:
			continue
		reg = re.compile(line, re.UNICODE | re.IGNORECASE)
		find.append((True, reg))
	for enc in test_encodings:
		if debug: stderr.write(u"Print test, now with %s:\n" % enc)
		correct = string.encode(enc)

		prog = string_program(argument = u"print")
		writer = Feeder(text = string, program = prog, encoding = enc)
		writer.start()
		raw = prog.stdout.read()
		prog.kill()
		try:
			result = raw.decode(enc)
		except UnicodeDecodeError:
			stderr.write(u"Could not decode return text,print line FAILED.\n")
			return False
		failed = False
		start, stop = 0, len(result)
		for req, reg in find:
			m = reg.search(result, start, stop)
			if not m and req:
				# Could not find a requierd line in this encoding
				try:
					if debug:
						stderr.write(u"Could not find requierd (encoding: %s):\n\t%s\n"
							% (enc, reg.pattern))
						stderr.write(u"Result was: '''\n%s'''\n" % result)
				except:
					stderr.write(u"!!! Could not print found string!\n")
					failed = True
				break
			if not m:
				continue
			f_start, f_stop = m.span()
			start = f_start
		if not failed:
			if debug:
				stderr.write(u"Print line test PASSED\n")
			else:
				stderr.write(u"Print line test FAILED\n")
			return True
	return False

def random_test(debug = True):
	if debug: stderr.write(u"\n#### Random line test:\n")
	string = u"[1]\n[2]\n[3]\n[4]\n[5]"
	str_len = len(string.split())
	line = {}
	total = 0
	failed = 0
	loops = 345
	numbers = re.compile(r'\[(\d+)\]')
	for x in xrange(loops):
		if x == int(loops / 2):
			sleep(.51)
		prog = string_program(argument = u"random")
		writer = Feeder(text = string, program = prog, encoding = u"ASCII")
		writer.start()
		raw = prog.stdout.readlines()
		lines = [x.strip() for x in raw if x.strip()]
		if len(lines) > 3:
			if debug: stderr.write(u"Found %d number of lines as output.\n" % len(lines))
			failed += 1
			break
		else:
			results = [int(x) for x in numbers.findall(u"\n".join(lines))]
		if len(results) != 1:
			failed += 1
			continue;
		result = results[0]
		if not line.has_key(result):
			line[result] = 0
		total += 1
		line[result] += 1
		writer.join()
		prog.kill()
		del(writer, prog)
	if failed > total:
		if debug: stderr.write(u"Failed %d tests larger than total passed %d.\n" % (failed, total))
		return False
	for key in line.keys():
		if line[key] >= total and str_len != 1:
			if debug: stderr.write(u"Line %d is always picked: FAILED\n" % key)
			return False
	pro = 1.0 / float(str_len)
	pro_ma = pro * 4.
	if pro_ma >= 1.:
		pro_ma = .99
	pro_mi = pro / 4.
	if pro_mi < .01:
		pro_mi = .01
	failed = False
	for key in line.keys():
		prob = float(line[key]) / float(total)
		if (pro_ma < prob) or (pro_mi > prob):
			if debug: stderr.write(u"Line %d has %5.2f%% distribution.\n" % (key, prob*100))
			failed = True
		else:
			if debug: stderr.write(u"Line %d has ok distribution %5.2f%%.\n" % (key, prob*100))
	if len(line) < str_len:
		if debug: stderr.write(u"Soft fail, not all lines were passed from randoms %d tries.\n" % loops)
		return True
	if failed and debug: stderr.write(u"Random line test SOFT failed\n")
	return True

def replace_test(string, debug=True):
	if debug: stderr.write(u"\n#### Replace vokal in lines test:\n")
	find = []
	for vokal in vokals_tot:
		for line in string.split('\n'):
			line = line.strip()
			if not line:
				continue
			reg = vokals_meng.sub(vokal, line)
			reg = vokals_mnor.sub(r"[%c\1]" % vokal, reg)
			reg = re.compile(reg, re.UNICODE | re.IGNORECASE)
			if vokal not in vokals_nor:
				find.append((True, vokal, reg))
			else:
				find.append((False, vokal, reg))
	for enc in test_encodings:
		if debug: stderr.write(u"Replace vokal test, now with %s:\n" % enc)
		prog = string_program(argument = u"replace")
		writer = Feeder(text = string, program = prog, encoding = enc)
		writer.start()
		raw = prog.stdout.read()
		prog.kill()
		try:
			result = raw.decode(enc)
		except UnicodeDecodeError:
			stderr.write(u" !!! Warning output was not %d encoded\n" % enc)
			result = raw
		failed = False
		opt = {}
		start, stop = 0, len(result)
		for req, vokal, reg in find:
			m = reg.search(result, start, stop)
			if not m and req:
				if debug:
					try:
						stderr.write(u"Could not find requierd (encoding: %s letter %s):\n\t%s\n"
						% (enc, vokal, reg.pattern))
					except:
						stderr.write(u"!!! Could not determine terminal CHARSET !!!\n")
				find_err = re.compile(vokals_match.sub(".", reg.pattern))
				m2 = find_err.search(result, start, stop)
				if m2:
					f1, f2 = m2.span()
					try:
						stderr.write(u"However, we found:\n\t%s\n" % result[f1:f2])
					except:
						stderr.write(u"Could not convert found string to terminal charset\n")
				# Could not find a requierd line in this encoding
				try:
					if debug:
						stderr.write(u"Result was: '''\n%s'''\n" % result)
				except:
					if debug:
						stderr.write(u"!!! Could not print result!\n")
				failed = True
				break
			if not m and not req:
				opt[vokal] = False
			if m and not req and not opt.has_key(vokal):
				opt[vokal] = True
			if not m:
				continue
			f_start, f_stop = m.span()
			start = f_start
		if not failed:
			if debug:
				try:
					for vokal in [v for v in opt.keys() if not opt[v]]:
						stderr.write(u"Optional %s NOT found.\n" % vokal)
					for vokal in [v for v in opt.keys() if opt[v]]:
						stderr.write(u"Optional %s found.\n" % vokal)
				except:
					stderr.write(u"!!! Could not determine terminal CHARSET !!!\n")
				stderr.write(u"Replace line test PASSED\n")
			return True
	if debug: stderr.write(u"Replace line test FAILED\n")
	return False

def remove_test(string, debug = True):
	if debug: stderr.write(u"\n#### Remove vokals test:\n")
	correct = vokals_meng.sub(u"", string)
	correct = vokals_mnor.sub(r"[\1]?", correct)
	find = []
	for line in correct.split('\n'):
		line = line.strip()
		if not line:
			continue
		reg = re.compile(line, re.UNICODE | re.IGNORECASE)
		find.append((True, reg))

	for enc in test_encodings:
		if debug: stderr.write(u"Remove vokals test, now with %s:\n" % enc)
		prog = string_program(argument = u"remove")
		writer = Feeder(text = string, program = prog, encoding = enc)
		writer.start()
		raw = prog.stdout.read()
		prog.kill()
		try:
			result = raw.decode(enc)
		except UnicodeDecodeError:
			stderr.write(u"Could not decode return text, remove vokals FAILED.\n")
			return False
		failed = False
		start, stop = 0, len(result)
		for req, reg in find:
			m = reg.search(result, start, stop)
			if not m and req:
				failed = True
				try:
					if debug:
						stderr.write(u"Could not find requierd (encoding: %s):\n\t%s\n"
							% (enc, reg.pattern))
						stderr.write(u"Result was: '''\n%s'''\n" % result)
				except:
					stderr.write(u"!!! Could not determine terminal CHARSET !!!\n")
				break
			if not m:
				continue
			f_start, f_stop = m.span()
			start = f_start
		if debug:
			if not failed:
				stderr.write(u"Remove vokals test PASSED\n")
			else:
				stderr.write(u"Remove vokals test FAILED\n")
		if not failed: return True
	return False

def len_test(string, debug = True):
	if debug: stderr.write(u"\n#### Length test:\n")
	numbers = re.compile(r'\d+')
	for enc in test_encodings:
		if debug: stderr.write(u"Length test, now with %s:\n" % enc)
		correct = len(string.encode(enc))
		prog = string_program(argument = u"len")
		writer = Feeder(text = string, program = prog, encoding = enc)
		writer.start()
		raw = prog.stdout.read()
		prog.kill()
		try:
			result = raw.decode(enc)
		except UnicodeDecodeError:
			result = raw
		nums = numbers.findall(result)
		nums_l = len(nums)
		if not nums_l:
			if debug: stderr.write(u"Length test failed, could not find a number!\n")
			return False
		if nums_l > 3:
			if debug: stderr.write(u"Length test failed, found %d numbers!\n" % nums_l)
			return False
		if unicode(correct) in nums:
			if debug: stderr.write(u"Length test PASSED!\n")
			return True
		else:
			if debug: stderr.write(u"Found numbers %s, correct is %d\n" % (unicode(nums), correct))
	if debug: stderr.write(u"Length test FAILED!\n")
	return False

if __name__ == '__main__':
	debug = True
	total = random_test(debug)
	for string in test_strings:
		result = True
		try:
			if debug: stderr.write(u"""\n$$$$ Testing:\t'''\n%s'''\n\n""" % string)
		except:
			pass
		result = print_test(string, debug) and result
		result = replace_test(string, debug) and result
		result = remove_test(string, debug) and result
		result = len_test(string, debug) and result
		total = total and result
		if debug:
			try:
				if result: stderr.write(u"\n@@@@ The string '''%s''' has\n\tPASSED!\n" % string.split('\n')[0])
				else: stderr.write(u"\nString '''%s'''\n\tFAILED\n" % string.split('\n')[0])
			except:
				pass
	if total: stdout.write(u"\nALL     PASS!\n")
	else: stdout.write(u"\nResult  FAILED\n")
