{
print("Number guessing game in SoftScript");
print();
print("by Ben Jones");
rand();rand();rand();rand();
number = (int(rand()*100));
guess = -1;
while(guess != number, {
	print("Guess an integer between 0 and 100:");
	guess = (int(input()));
	print();
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
}()