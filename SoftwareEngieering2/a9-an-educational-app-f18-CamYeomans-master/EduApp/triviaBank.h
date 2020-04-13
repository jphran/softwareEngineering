#ifndef TRIVIABANK_H
#define TRIVIABANK_H
#include <vector>
#include "triviaQuestion.h"
#include <json.hpp>
#include "iostream"
#include "fstream"
#include "QDebug"

class TriviaBank
{
std::vector<TriviaQuestion> questionList;
unsigned size = 0;

public:
    TriviaBank(std::string location);
    void AddQuestion(TriviaQuestion newQuestion);
    TriviaQuestion GetQuestion(unsigned index);
    unsigned GetSize();
    void RemoveQuestion(unsigned index);
};

#endif // TRIVIABANK_H
