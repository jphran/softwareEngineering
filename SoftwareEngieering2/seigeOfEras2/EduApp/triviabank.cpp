#include "triviaBank.h"

TriviaBank::TriviaBank(std::string location)
{
    // read a JSON file
    std::string filename = "../EduApp/trivia.json";
    std::ifstream file(filename);
    nlohmann::json data;
    file >> data;

    if(location == "England")
    {
        for(nlohmann::json::iterator i = data.begin(); i != data.end(); ++i)
        {
            nlohmann::json question = *i;
            if(question["LOCATION"].get<std::string>() == "England")
            {
                std::vector<std::string> possibleAns;
                possibleAns.push_back(question["ANSWER1"]);
                possibleAns.push_back(question["ANSWER2"]);
                possibleAns.push_back(question["ANSWER3"]);
                possibleAns.push_back(question["ANSWER4"]);
                TriviaQuestion englandTriviaQ(question["LOCATION"],question["QUESTION"],question["CORRECTANSWER"],possibleAns);
                this->AddQuestion(englandTriviaQ);
            }
        }
    }
    else if (location == "France")
    {
        for(nlohmann::json::iterator i = data.begin(); i != data.end(); ++i)
        {
            nlohmann::json question = *i;
            if(question["LOCATION"] == "France"){
                std::vector<std::string> possibleAns;
                possibleAns.push_back(question["ANSWER1"]);
                possibleAns.push_back(question["ANSWER2"]);
                possibleAns.push_back(question["ANSWER3"]);
                possibleAns.push_back(question["ANSWER4"]);
                TriviaQuestion franceTriviaQ(question["LOCATION"],question["QUESTION"],question["CORRECTANSWER"],possibleAns);
                this->AddQuestion(franceTriviaQ);
            }
        }
    }
    else if(location == "Greece")
    {
        for(nlohmann::json::iterator i = data.begin(); i != data.end(); ++i)
        {
            nlohmann::json question = *i;
            if(question["LOCATION"] == "Greece")
            {
                std::vector<std::string> possibleAns;
                possibleAns.push_back(question["ANSWER1"]);
                possibleAns.push_back(question["ANSWER2"]);
                possibleAns.push_back(question["ANSWER3"]);
                possibleAns.push_back(question["ANSWER4"]);
                TriviaQuestion greeceTriviaQ(question["LOCATION"],question["QUESTION"],question["CORRECTANSWER"],possibleAns);
                this->AddQuestion(greeceTriviaQ);
            }
        }
    }
    else if(location == "Rome")
    {
        for(nlohmann::json::iterator i = data.begin(); i != data.end(); ++i)
        {
            nlohmann::json question = *i;
            if(question["LOCATION"] == "Rome")
            {
                std::vector<std::string> possibleAns;
                possibleAns.push_back(question["ANSWER1"]);
                possibleAns.push_back(question["ANSWER2"]);
                possibleAns.push_back(question["ANSWER3"]);
                possibleAns.push_back(question["ANSWER4"]);
                TriviaQuestion romeTriviaQ(question["LOCATION"],question["QUESTION"],question["CORRECTANSWER"],possibleAns);
                this->AddQuestion(romeTriviaQ);
            }
        }
    }
    else
    {
        for(nlohmann::json::iterator i = data.begin(); i != data.end(); ++i)
        {
            nlohmann::json question = *i;
            std::vector<std::string> possibleAns;
            possibleAns.push_back(question["ANSWER1"]);
            possibleAns.push_back(question["ANSWER2"]);
            possibleAns.push_back(question["ANSWER3"]);
            possibleAns.push_back(question["ANSWER4"]);
            TriviaQuestion allTriviaQ(question["LOCATION"],question["QUESTION"],question["CORRECTANSWER"],possibleAns);
            this->AddQuestion(allTriviaQ);
        }
    }
}

void TriviaBank::AddQuestion(TriviaQuestion newQuestion)
{
    size = size + 1;
    questionList.push_back(newQuestion);
}

TriviaQuestion TriviaBank::GetQuestion(unsigned index)
{
    return questionList[index];
}

unsigned TriviaBank::GetSize()
{
    return size;
}

void TriviaBank::RemoveQuestion(unsigned index)
{
    questionList.erase(questionList.begin()+index);
    size = size - 1;
}
