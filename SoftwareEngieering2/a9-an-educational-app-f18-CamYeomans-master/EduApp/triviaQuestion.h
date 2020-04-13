#ifndef TRIVIAQUESTION_H
#define TRIVIAQUESTION_H
#include <vector>
#include <string>

class TriviaQuestion {
    std::string location;
	std::string question;
	std::string correctAnswer;
    std::vector<std::string> answers;
	
public:
    TriviaQuestion();
    TriviaQuestion(std::string newLocation,std::string newQuestion,std::string newCorrectAnswer,std::vector<std::string> newAnswers);
	std::string getLocation();
	std::string getQuestion();
	std::string getCorrect();
    std::vector<std::string> getAnswers();
    bool answerQuestion(std::string guess);
};

#endif
