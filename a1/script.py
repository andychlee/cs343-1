x = []
y = []
# with open('r1') as r1:
r1 = open('r1', 'r')
r1_copy = open('r1_copy', 'w')
for line in r1:
	line = line.replace('~', '')
	r1_copy.write(line)
r1.close()
r1_copy.close()


r2 = open('r2', 'r')
r2_copy = open('r2_copy', 'w')
for line in r2:
	line = line.replace('~', '')
	r2_copy.write(line)
r2.close()
r2_copy.close()

# print(x)
# print(y)
# for i in range(len(x)):
# 	if x[i] != y[i]:
# 		print('ss')