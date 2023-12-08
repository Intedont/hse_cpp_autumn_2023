from faker import Faker
from random import randint
import json
import ujson
import cjson
import time

def generate(n):
	faker = Faker()
	data = []
	for _ in range(n):
		key = faker.name()
		way = randint(1, 3)
		if way == 1:
			val = faker.address()
		elif way == 2:
			val = faker.text()
		else:
			val = randint(1, 10**9)
		
		data_to_append = f'"{key}": {val}' if way == 3 else f'"{key}": "{val}"'

		# Не знаю почему, но факер рандомно генерирует символы переноса строки, что ломает парсеры.
		# Поэтому удаляем символы вручную
		data_to_append = data_to_append.replace('\n', ' ') 

		data.append(data_to_append)

	
	return '{' + ', '.join(data) + '}'

def main():	
	d = generate(100000)
	print("Data is generated")

	f = open('benchmarks.txt', 'w')
	
	start = time.time()
	json_dict = json.loads(d)
	end = time.time() - start
	print(f'json loads time is: {end} ')
	f.write(f'json loads time is: {end} \n')
	start = time.time()
	json.dumps(json_dict)
	end = time.time() - start
	print(f'json dumps time is: {end} ')
	f.write(f'json dumps time is: {end} \n\n')

	start = time.time()
	json_dict = ujson.loads(d)
	end = time.time() - start
	print(f'ujson loads time is: {end} ')
	f.write(f'ujson loads time is: {end} \n')
	start = time.time()
	ujson.dumps(json_dict)
	end = time.time() - start
	print(f'ujson dumps time is: {end} ')
	f.write(f'ujson dumps time is: {end} \n\n')
	
	start = time.time()
	json_dict = cjson.loads(d)
	end = time.time() - start
	print(f'cjson loads time is: {end} ')
	f.write(f'cjson loads time is: {end} \n')
	start = time.time()
	cjson.dumps(json_dict)
	end = time.time() - start
	print(f'cjson dumps time is: {end} ')
	f.write(f'cjson dumps time is: {end} \n\n')

	f.close()


if __name__ == "__main__":
    main()
