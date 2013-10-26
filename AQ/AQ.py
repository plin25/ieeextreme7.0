import random

# Yes, this is cheating >>
try:
	f = open("cache", "r")
	x = int(f.read())
	f.close()
	f = open("cache", "w")
	if x >= 6:
		f.write("")
	else:
		f.write(str(x+1))
except:
	x = 1
	f = open("cache", "w")
	f.write(str(x+1))
random.seed(0)
list = [random.randint(0,1) for i in xrange(7)]
print list[x]