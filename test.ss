{
print("Number guessing game in SoftScript");
print();
number = (int(rand()*100));
guess = -1;
while(guess != number, {
	b = 1;
	print("Guess an integer between 0 and 100:");
	guess = (int(input()));
	print();
	if(guess > 100, {
		print("Guess is not between 0 and 100");
		b = 0;
	});
	if(guess < 0, {
		print("Guess is not between 0 and 100");
		b = 0;
	});
	if(b, {
		if(guess > number, {
			print("Guess is too high!");
		});
		if(guess < number, {
			print("Guess is too low!");
		});
		if(guess == number, {
			print("You guessed the number!");
		});
	});
});
}()