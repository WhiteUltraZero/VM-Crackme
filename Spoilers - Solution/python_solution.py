def generate_password():
	secret_char = 66
	password = []

	for index in range(6):

		cur_char = secret_char + (index + 1)

		secret_char += 4
		password.append(chr(cur_char))

	return ''.join(password)


if __name__ == "__main__":
	result = generate_password()
	print("Password:", result)