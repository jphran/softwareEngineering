#include "triviaQuestion.h"

TriviaQuestion::TriviaQuestion()
{
    std::vector<std::string> tempArray =  {"A","B","C","D"};
    location = "All";
    question = "Temp";
    correctAnswer = "A";
    answers = tempArray;
}

TriviaQuestion::TriviaQuestion(std::string newLocation,std::string newQuestion,std::string newCorrectAnswer,std::vector<std::string> newAnswers)
{
	location = newLocation;
	question = newQuestion;
	correctAnswer = newCorrectAnswer;
	answers = newAnswers;	
}

std::string TriviaQuestion::getLocation()
{
    return location;
}
	
std::string TriviaQuestion::getQuestion()
{
    return question;
}

std::string TriviaQuestion::getCorrect()
{
	return correctAnswer;
}

std::vector<std::string> TriviaQuestion::getAnswers()
{
	return answers;
}

bool TriviaQuestion::answerQuestion(std::string guess)
{
    if(guess == correctAnswer){
        return true;
    }else{
        return false;
    }
}
