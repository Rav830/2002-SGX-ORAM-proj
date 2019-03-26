from faker import Faker
import random

fake = Faker()
fake.seed(4321)

numCustomers = 10
numOrders = 200


ch = open('customer.csv', 'w')
oh = open('order.csv', 'w')
ch.write("id,name,expire_time\n")
oh.write("id,name,expire_time\n")


print("creating customers")
for i in range(numCustomers):
	ch.write("%s,%s,%s\n" % (i, fake.name(), 255));
	
	
print("creating orders")

for i in range(numOrders):
	oh.write("%s,%s,%s\n" % (
		random.randint(0,numCustomers), 
		fake.text(max_nb_chars=49), 
		random.randint(0,256))
	);
	
ch.close()
oh.close()