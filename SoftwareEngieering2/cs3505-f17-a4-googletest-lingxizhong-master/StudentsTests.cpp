/*
Assignment 04 - Travis and Google TEST
Lingxi Zhong and Osama Kergaye
U0770136 | u0767339

*/

#include "Students.h"
#include "gtest/gtest.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv) {
  printf("Running main() from gtest_main.cc\n");
  testing::InitGoogleTest(&argc, argv);
  RUN_ALL_TESTS();
  return 0;
}


// Add User Tests:
TEST(addUsers, addUserBasicTests) {
  Students stu;
  string testUser = "Bob";
  stu.addUser(testUser, 1);
  EXPECT_TRUE(stu.nameExists(testUser));
  stu.addUser("TestString", 2);
  EXPECT_TRUE(stu.nameExists("TestString"));
  EXPECT_EQ(2, stu.numberOfNames());
}

TEST(addUsers, addUserDupID) {
  Students stu;
  stu.addUser("Charlie", 1);
  stu.addUser("Tango", 1);
  EXPECT_EQ(1, stu.idForName("Charlie"));
  EXPECT_EQ(1, stu.idForName("Tango"));
}

TEST(addUsers, addUserStressTest) {
  Students stu;
  for(int i = 0; i < 100000; i++) {
    stu.addUser("user"+to_string(i), i);
  }
  for(int j = 0; j < 100000; j++) {
    string usernum = "user" + to_string(j);
    EXPECT_TRUE(stu.nameExists(usernum));
  }
}

TEST(addUsers, addUserStressTest1Mill) {
  Students stu;
  for(int i = 0; i < 1000000; i++) {
    stu.addUser("user"+to_string(i), i);
  }
  for(int j = 0; j < 1000000; j++) {
    string usernum = "user" + to_string(j);
    EXPECT_TRUE(stu.nameExists(usernum));
  }
}

TEST(addUsers, addUserWithSameName) {
  Students stu;
  stu.addUser("user", 1);
  stu.addUser("user", 2);
  EXPECT_EQ(1, stu.idForName("user"));
}

TEST(addUsers, addUserWithBlankString) {
  Students stu;
  stu.addUser("", 1);
  string empty;
  stu.addUser(empty, 1);
  EXPECT_EQ(0, stu.numberOfNames());
}

//AddPhoneNumber Tests
TEST(addPhoneNumbers, addPhoneNumbersBasic) {
  Students stu;
  stu.addUser("user", 1);
  stu.addPhoneNumbers(1, "123-456-7890");
  EXPECT_EQ("123-456-7890", stu.phoneForName("user"));
}

TEST(addPhoneNumbers, addPhoneNumberStress) {
  Students stu;
  for (int i = 0; i < 100000; i++) {
    stu.addUser("user"+to_string(i), i);
  }
  for (int j = 0; j < 100000; j++) {
    stu.addPhoneNumbers(j, "123-456-7890");
  }
  for (int h = 0; h < 100000; h++) {
    EXPECT_EQ("123-456-7890", stu.phoneForName("user"+(to_string(h))));
  }
}

//Add Grade Tests
TEST(addGrade, basicFunctionality) {
  Students stu;
  stu.addUser("user", 1);
  stu.addGrade(1, 'A');
  EXPECT_EQ('A', stu.gradeForName("user"));
}

TEST(addGrade, addGradeStressTest) {
  Students stu;
  for(int i = 0; i < 100000; i++) {
    stu.addUser("user"+to_string(i), i);
    stu.addGrade(i, 'F'); // FAIL EVERYONE >:D
  }
  for(int j = 0; j < 100000; j++) {
    EXPECT_EQ('F', stu.gradeForName("user"+(to_string(j))));
  }
}

// id For Names
TEST(idForNames, basicFunctionality) {
  Students stu;
  stu.addUser("user", 1);
  EXPECT_EQ(1, stu.idForName("user"));
}

TEST(idForNames, stressTest) {
  Students stu;
  for(int i = 0; i  < 100000; i++) {
    stu.addUser("user"+to_string(i), i);
  }
  for(int j = 0; j < 100000; j++) {
    ASSERT_EQ(j, stu.idForName("user"+(to_string(j))));
  }
}

TEST(idForNames, exceptionTest) {
  Students stu;
  EXPECT_THROW(stu.idForName("user"), std::out_of_range);
}

//Grade For Name

TEST(gradeForName, basicFunctionality) {
  Students stu;
  stu.addUser("user", 1);
  stu.addGrade(1, 'A');
  EXPECT_EQ('A', stu.gradeForName("user"));
}

TEST(gradeForName, stressTest) {
  Students stu;
  for (int i = 0; i < 100000; i++) {
    stu.addUser("user"+to_string(i), i);
    stu.addGrade(i, 'F');
  }
  for (int j = 0; j < 100000; j++) {
    EXPECT_EQ('F', stu.gradeForName("user" + (to_string(j))));
  }
}

TEST(gradeForName, exceptionTest) {
  Students stu;
  stu.addUser("user", 1);
  EXPECT_THROW(stu.gradeForName("user"), std::out_of_range);
  EXPECT_THROW(stu.gradeForName("user2"), std::out_of_range);
}

// Phone for Name

TEST(phoneForName, basicFunctionality) {
  Students stu;
  stu.addUser("user", 1);
  stu.addPhoneNumbers(1, "123-456-7890");
  EXPECT_EQ("123-456-7890", stu.phoneForName("user"));
}

TEST(phoneForName, phoneForNameStress) {
  Students stu;
  for (int i = 0; i < 100000; i++) {
    stu.addUser("user"+to_string(i), i);
  }
  for (int j = 0; j < 100000; j++) {
    stu.addPhoneNumbers(j, "123-456-7890");
  }
  for (int h = 0; h < 100000; h++) {
    EXPECT_EQ("123-456-7890", stu.phoneForName("user"+(to_string(h))));
  }
}

TEST(phoneForName, phoneForNameException) {
  Students stu;
  EXPECT_THROW(stu.phoneForName("user"), std::out_of_range);
}

// Name Exists

TEST(nameExists, basicFunctionality) {
  Students stu;
  stu.addUser("user", 1);
  EXPECT_TRUE(stu.nameExists("user"));
  EXPECT_FALSE(stu.nameExists("user2"));
}

TEST(nameExists, edgeCaseoFID) {
  Students stu;
  stu.addUser("user", 123);
  EXPECT_FALSE(stu.nameExists("123"));
}

//full record

TEST(fullRecord, fullRecordFalseTest) {
  Students stu;
  string user;
  unsigned int id;
  string phone;
  char grade;
  EXPECT_FALSE(stu.fullRecord(user,id,phone,grade));
}

TEST(fullRecord, fullRecordWithInfo) {
  Students stu;
  stu.addUser("user", 1);
  stu.addPhoneNumbers(1, "123-456-7890");
  stu.addGrade(1, 'A');
  string user = "user";
  unsigned int id;
  string phone;
  char grade;
  EXPECT_TRUE(stu.fullRecord(user, id, phone, grade));
  EXPECT_EQ(1, id);
  EXPECT_EQ("123-456-7890", phone);
  EXPECT_EQ('A', grade);
}

// Number of names
TEST(numberOfNames, numberOfNamesStressTest) {
  Students stu;
  for(int i = 0; i < 100000; i++) {
    stu.addUser("user"+to_string(i), i);
  }
  for(int j = 0; j < 100000; j++) {
    EXPECT_TRUE(stu.nameExists("user"+(to_string(j))));
  }
  EXPECT_EQ(100000, stu.numberOfNames());
}

// End of Lingxi tests, start of Osama Tests ------------


//a bredth test of clearAll
TEST(clearAll, bulkTestToEnsureClearGotAllData){
  Students stu;
  EXPECT_EQ(0, stu.numberOfNames());
  stu.addUser("sambini", 001);
  EXPECT_EQ(1, stu.numberOfNames());
  stu.addGrade(001, 'A');
  EXPECT_EQ('A', stu.gradeForName("sambini"));
  stu.addPhoneNumbers(001, "801-801-1234");
  EXPECT_EQ("801-801-1234", stu.phoneForName("sambini"));
  stu.clearAll();
  EXPECT_EQ(0, stu.numberOfNames());
  EXPECT_FALSE(stu.nameExists("sambini"));
  EXPECT_THROW(stu.removeStudent("sambini"), out_of_range);
  //this error allready discoverd by Lingxi
  //EXPECT_THROW(stu.phoneForName("sambini"), out_of_range);
  EXPECT_THROW(stu.gradeForName("sambini"), out_of_range);
  EXPECT_THROW(stu.removeStudent("sambini"), out_of_range);
  vector<string> names;
  names.push_back("sambini");
  EXPECT_EQ(0, stu.removeList(names));
}

//small stress test for removeList via adding 100, then removeing 100
TEST(removeList, 100PeopleRemoveTest){
  Students stu;
  EXPECT_EQ(0, stu.numberOfNames());
  string user = "A";
  vector<string> names;
 for(int i = 0; i <100; i++){
   stu.addUser(user, i);
   names.push_back(user);
   user.push_back(i);
 }
 EXPECT_EQ(100, stu.numberOfNames());
 EXPECT_EQ(100, stu.removeList(names));
 EXPECT_EQ(0, stu.removeList(names));
 EXPECT_EQ(0, stu.numberOfNames());
}

//small stress test for removeList when it has nonsence in the vector
TEST(removeList, 100PeopleRemoveTestWithNonsence){
  Students stu;
  EXPECT_EQ(0, stu.numberOfNames());
  string user = "A";
  vector<string> names;
  names.push_back("");
  names.push_back("34567890");
 for(int i = 0; i <100; i++){
   stu.addUser(user, i);
   names.push_back(user);
   user.push_back(i);
 }
 EXPECT_EQ(100, stu.numberOfNames());
 //error found, if the vector has nonsence in it, it returns 0 instead
 EXPECT_EQ(100, stu.removeList(names));
 EXPECT_EQ(0, stu.removeList(names));
 EXPECT_EQ(0, stu.numberOfNames());
}

//StressTest by adding then removeing each students
TEST(removeStudent, 100studentRemoveTestWithoutNumberOrGradeAlternating){
  Students stu;
  EXPECT_EQ(0, stu.numberOfNames());
  string user = "A";
  vector<string> names;
 for(int i = 0; i <100; i++){
   stu.addUser(user, i);
   stu.removeStudent(user);
   EXPECT_EQ(0, stu.numberOfNames());
   names.push_back(user);
   user.push_back(i);
 }
 EXPECT_EQ(0, stu.removeList(names));
 EXPECT_EQ(0, stu.numberOfNames());
}


//SressTest by adding then removeing each students and adding a grade
TEST(removeStudent, 100studentRemoveTestWithoutNumberAlternating){
  Students stu;
  EXPECT_EQ(0, stu.numberOfNames());
  string user = "A";
 for(int i = 0; i <100; i++){
   stu.addUser(user, i);
   stu.addGrade(i, 'A');
   EXPECT_EQ('A', stu.gradeForName(user));
   stu.removeStudent(user);
   EXPECT_EQ(0, stu.numberOfNames());
   EXPECT_THROW(stu.gradeForName(user), out_of_range);
   user.push_back(i);
 }
 EXPECT_EQ(0, stu.numberOfNames());
}


//SressTest by adding then removeing each students and adding a grade and number
TEST(removeStudent, 100studentRemoveTestWithAllInfoAlternating){
  Students stu;
  EXPECT_EQ(0, stu.numberOfNames());
  string user = "A";
  string number = "0";
 for(int i = 0; i <10; i++){
   stu.addUser(user, i);
   stu.addPhoneNumbers(i, number);
   stu.addGrade(i, 'A');
   stu.removeStudent(user);
   EXPECT_EQ(0, stu.numberOfNames());
   EXPECT_THROW(stu.gradeForName(user), out_of_range);
   //stu.phoneForName(user);
   //EXPECT_EQ(number, stu.phoneForName(user));
   user.push_back(i);
   number.push_back(i);
 }
 EXPECT_EQ(0, stu.numberOfNames());
}

//Tesing edge case for removing a non existant user
TEST(removeStudent, removeNonUser){
  Students stu;
  EXPECT_EQ(0, stu.numberOfNames());
  string user = "A";
  string number = "0";
  int i = 1;
   stu.addUser(user, i);
   stu.addPhoneNumbers(i, number);
   stu.addGrade(i, 'A');
  // EXPECT_EQ('A', stu.gradeForName(user));
   EXPECT_THROW(stu.removeStudent("user"), out_of_range);

   EXPECT_EQ(1, stu.numberOfNames());

}

//edge case testing for whitespace removal
TEST(removeStudent, removeWhiteSpace){
  Students stu;
   EXPECT_THROW(stu.removeStudent(""), out_of_range);
   EXPECT_EQ(0, stu.numberOfNames());
}

//removing valid whitespace
TEST(removeStudent, removeWhiteSpaceAfterAddingWhiteSpace){
  Students stu;
  stu.addUser("", 0);
  stu.removeStudent("");
  EXPECT_EQ(0, stu.numberOfNames());
}

//Error found here phoneforname increments numberOfNames
TEST(phoneForName, seemsToAddOneToNumberOfNames){
  Students stu;
  EXPECT_EQ(0, stu.numberOfNames());
  stu.phoneForName("user");
  EXPECT_EQ(0, stu.numberOfNames());
}
