#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int BOX_SIZE = 100;
const int BOX_COUNT = 5;
const int NUMBER_RANGE = 100;  // Note, need to 
const int VERSION = 5;

// generateRandomNumbers generates 4 randoms numbers.
void generateRandomNumbers(int numbers[]) {
    srand(time(0)); // Seed the random number generator with current time
	int j;
    for (int i = 0; i < BOX_COUNT; i++) {
        numbers[i] = rand() % 100; // Generate random numbers from 0 to 99
		j = i - 1;
		while(j>-1)
		{
			if(numbers[i] == numbers[j])
			{
				numbers[i] = rand() % 100; 
				j = i;
			}
			j--;
		}
    }
}

// DEBUG FUNCTIONS
// outputNumbers displays numbers array to std::cout
void outputNumbers(int numbers[]) {

	for (int i = 0; i < BOX_COUNT; i++) 
	{
		std::cout << "Numbers[" << i << "] = " << numbers[i] << std::endl;
	}

}

// END DEBUG FUNCTIONS

// ==== CreateSumInNumbers ====
//  Function changes one number to a number that is the sum of 2 of the other numbers.
int CreateSumInNumbers(int numbers[]) 
{
	// Check the number of numbers which add up to another number.
	// if the number is 1, then stop and return the number of the box with the integer in it.
	// if the number is 0, then sum two random numbers and call function again, return the result.
	// if the number is >1, then take one of the numbers which is the result of a sum and change it to another number.  (make sure its not a duplicate as abolve)
	//    then call this function and return the result.
	// 
	//  NOTE:  An error returns -1 (or another negative number)
	int sum_count = 0;
	int sum_index = -1;
	
	for (int h = 0; h < BOX_COUNT; h++) 
	{
		for (int i = h+1; i < BOX_COUNT; i++) 
		{
			for (int j = 0; j < BOX_COUNT; j++) 
			{
				if( j != h && j != i )  // obviously skip self references
				{
					if(numbers[h] + numbers[i] == numbers[j]) // we found a sum.
					{
						if(sum_count == 0)
						{	
							sum_count++;
							sum_index = j;
						}
						else // sum_count == 1 and we found another sum so we randomly replace one of the sums
						{
							// NOTE: By not finding all of the sums before deciding which one to swap out this may skew the over all 
							// sum_index returns towards the later numbers which might make it easier.
							// since I am only planning on using this for 4 or 5 numbers at the moment I'm not going to worry about it.  
							// I think this could be re-written to find all of the indexes and randomly select from them if needed.
							int to_replace_index = -1;
							if((rand() % 100)>50)
								{to_replace_index = sum_index;} // replace sum_index (previously found sum)
							else
								{to_replace_index = sum_index;} // replace j (recently found sum)
							
							// replace the number
							numbers[to_replace_index] = rand() % 100; 
							
							// check for duplicate numbers (maybe I should have just used a hash for this)
							int l;
							// for (int k = 1; k < BOX_COUNT; k++) 
							for (int k = to_replace_index; k < BOX_COUNT; k++)  // think should be OK here since numbers before to_replace_index are already de-duped and there are no scenarios where earlier numbers get changed.
							{
								l = k - 1;
								while(l>-1)
								{
									if(numbers[k] == numbers[l])
									{
										numbers[k] = rand() % 100; 
										// decrement k so that it goes through the loop again and compares all of the numbers for this 
										// one again to make sure we haven't collided with a previously compared number.
										k--;
									}
									l--;
								}
							}
							// completed duplicate check now call function again for recursion to remove
							// need to restart the sum check since we changed a number, but we can assert that
							// when this function returns there will be only one sum in the list of numbers
							// hence we'll break out of these loops here (Also we don't know if we are before or
							// after the operands that sum to the sum_index value so we don't know what to set sum_count 
							// equal to to ensure it ends up as 1 at the end of the run if it continues)
							sum_index = CreateSumInNumbers(numbers);
							
							// sum_count = 1; but its already set 
							
							// *** BRS 20230831: ADD DEBUG here to dump sum_index and numbers array to cmd prompt ***
							outputNumbers(numbers);
							// Debug
							
							break;
						}
					}
				}
			}
		}
	}
	
	// if the sum_count is 1, return the sum_index
	if(sum_count ==1)
	{
		return sum_index;
	}
	else // sum_count must be 0
	{
		// sum two random numbers (ensure the result is below the MAX RANDOM or its a dead giveaway)
		int index_1 = rand() % BOX_COUNT;
		int index_2 = rand() % BOX_COUNT;
		int index_3 = rand() % BOX_COUNT;
		while(index_1 == index_2 || index_2 == index_3 || index_1 == index_3)
		{ 
			index_2 = rand() % BOX_COUNT;
			index_3 = rand() % BOX_COUNT;
		}
		// now we should have 3 unique indexes
		// Debug
		std::cout << "D: Indexes 1: " << index_1 << " 2: " << index_2 << " 3: " << index_3 << std::endl;
		outputNumbers(numbers);
		
		// Assign index_3  as the sum of index_1 & 2 since they were selected randomly it should really matter
		// Also need to check and make sure sum is less than NUMBER_RANGE
		numbers[index_3] = numbers[index_1] + numbers[index_2];
		if(numbers[index_3] >= (NUMBER_RANGE +  NUMBER_RANGE / 2))
		{
			// Need to reduce both numbers because reducing 1 by 1/2 won't bring it below NUMBER_RANGE
			numbers[index_1]  = numbers[index_1]  - int(NUMBER_RANGE / 2);  // do I need to use floor here?
			numbers[index_2]  = numbers[index_2]  - int(NUMBER_RANGE / 2);  //  ** Would sequential numbers result in rounding to the same number?
			numbers[index_3] = numbers[index_1] + numbers[index_2];  // if we see duplicate numbers can put a check above here I think.
		}
		else if(numbers[index_3] >= NUMBER_RANGE)
		{
			// Need to reduce larger of the two numbers to bring sum under NUMBER_RANGE
			if( numbers[index_1]  > numbers[index_2] )
			{
				numbers[index_1]  = numbers[index_1]  - int(NUMBER_RANGE / 2);  // do I need to use floor here?
			
			}
			else
			{
				numbers[index_2]  = numbers[index_2]  - int(NUMBER_RANGE / 2);
			}
			numbers[index_3] = numbers[index_1] + numbers[index_2]; 
		}
			
			
		
		// recursive call to make sure non of the results 
		sum_index = CreateSumInNumbers(numbers);
		
		// *** Not certain if this is correct, picking the code back up after a month or so.
		return sum_index;
	}
	
}

int main() {
	std::cout << "Version: " <<  VERSION << std::endl;
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Random Numbers in Boxes");

    int numbers[BOX_COUNT];
    generateRandomNumbers(numbers);
	int sum_index = -1;
	sum_index = CreateSumInNumbers(numbers);
	int score = 0;
	bool guessOneFlag = true; 
	// User can score up to 100 points by answering.  
	// 10 points is the minimum score.  
	// Bonus points are awared for selecting the correct answer quickly.  
	// Past first second score is reduced by 1 for each 1/10 of a second

	// Create a timer to track how long it is until the users clicks an answer
	sf::Clock timer;
	float elapsedTime = 0.0f;

    sf::Font font;
    if (!font.loadFromFile("FreeMono.ttf")) {
        std::cerr << "Error loading font file!" << std::endl;
        return 1;
    }
	
	// Draw instruction window
	sf::Text topLeftText;
    topLeftText.setFont(font);
    topLeftText.setCharacterSize(18);
    topLeftText.setFillColor(sf::Color::Red);
    topLeftText.setString("Instructions:  Click the box of the number which two other\nnumbers add to.  For example the correct answer for numbers \n2, 3, 4, 5 is 5 because 2 + 3 = 5");
    topLeftText.setPosition(10.0f, 10.0f);
	
	// Draw points window
	sf::Text bottomLeftText;
    bottomLeftText.setFont(font);
    bottomLeftText.setCharacterSize(24);
    bottomLeftText.setFillColor(sf::Color::Blue);
    bottomLeftText.setString("                Points: " + std::to_string(score));
    bottomLeftText.setPosition(10.0f, static_cast<float>(WINDOW_HEIGHT) - 10.0f - topLeftText.getGlobalBounds().height);
	//bottomLeftText.setPosition( 10.0f, WINDOW_HEIGHT - 20.0f);

    sf::RectangleShape boxes[BOX_COUNT];
    sf::Text buttonText[BOX_COUNT];
	

    for (int i = 0; i < BOX_COUNT; i++) {
        int x = i * (BOX_SIZE + 20) + 20; // Calculate the position of the boxes
        int y = WINDOW_HEIGHT / 2 - BOX_SIZE / 2;
        boxes[i].setPosition(x, y);
        boxes[i].setSize(sf::Vector2f(BOX_SIZE, BOX_SIZE));
        boxes[i].setFillColor(sf::Color::White);
        boxes[i].setOutlineThickness(2);
        boxes[i].setOutlineColor(sf::Color::Black);

        buttonText[i].setFont(font);
        buttonText[i].setCharacterSize(42);
        buttonText[i].setFillColor(sf::Color::Black);
        buttonText[i].setString(std::to_string(numbers[i]));

        sf::FloatRect textRect = buttonText[i].getLocalBounds();
        buttonText[i].setOrigin(textRect.left + textRect.width / 2.0f,
                                textRect.top + textRect.height / 2.0f);
        buttonText[i].setPosition(x + BOX_SIZE / 2.0f, y + BOX_SIZE / 2.0f);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    for (int i = 0; i < BOX_COUNT; i++) {
                        if (boxes[i].getGlobalBounds().contains(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y))) {
                            // The button with index i is clicked
                            std::cout << "Button " << i + 1 << " is clicked! Number: " << numbers[i] << std::endl;
							
							if(i == sum_index)
							{
								elapsedTime = timer.getElapsedTime().asMilliseconds();
								std::cout << " * Correct! * " << std::to_string(elapsedTime)  << std::endl;
								 generateRandomNumbers(numbers);
								sum_index = -1;
								sum_index = CreateSumInNumbers(numbers);
								// Calculate score:
								if(!guessOneFlag)
								{
									score +=10;
								}
								else
								{
									if(elapsedTime < 1000)
									{
										score +=100;
									}
									else if(elapsedTime < 10000)
									{
										score = score  + 10 + int((10000 - elapsedTime)/100);
									}
									else
									{
										score +=10;
									}
								}
								
								// Update screen text
								bottomLeftText.setString("Correct!        Points: " + std::to_string(score));
								for (int i = 0; i < BOX_COUNT; i++) 
								{
									buttonText[i].setString(std::to_string(numbers[i]));
								}
								timer.restart();
								guessOneFlag = true;
							}
							else
							{
								std::cout << " * Incorrect! * " << std::endl;
								guessOneFlag = false;
								bottomLeftText.setString("Incorrect!  Try Again.   Points: " + std::to_string(score));
							}
                        }
                    }
                }
            }
        }

        window.clear(sf::Color::White);

        for (int i = 0; i < BOX_COUNT; i++) {
            window.draw(boxes[i]);
            window.draw(buttonText[i]);
        }
		window.draw(topLeftText);
		window.draw(bottomLeftText);

        window.display();
    }

    return 0;
}
