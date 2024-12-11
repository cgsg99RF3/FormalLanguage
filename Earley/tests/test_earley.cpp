#include <gtest/gtest.h>
#include <stdexcept>
#include "../src/earley.h"

TEST(EarleyAlgTest, InvalidInput) {
  Earley earley;
  EXPECT_ANY_THROW(fit_for_test(earley.grammar, "1 3 1\nS\naSB\nS->abS\nS"));
  EXPECT_ANY_THROW(fit_for_test(earley.grammar, "1 1 1\nS\nab\nS->abS\nS"));
  EXPECT_ANY_THROW(fit_for_test(earley.grammar, "1 2 1\nS\nab\nS->abS\nf"));
}

TEST(EarleyAlgTest, AcceptWordTest1) {
  Earley earley;
  fit_for_test(earley.grammar, "1 2 2\nS\nab\nS->aSbS\nS ->\nS");
  EXPECT_EQ(earley.Predict("aababb"), true);
  EXPECT_EQ(earley.Predict("ab"), true);
  EXPECT_EQ(earley.Predict(""), true);
  EXPECT_EQ(earley.Predict("abababab"), true);
}

TEST(EarleyAlgTest, RejectWordTest1) {
  Earley earley;
  fit_for_test(earley.grammar, "1 2 2\nS\nab\nS->aSbS\nS ->\nS");
  EXPECT_EQ(earley.Predict("aabbba"), false);
  EXPECT_EQ(earley.Predict("bbb"), false);
  EXPECT_EQ(earley.Predict("cdkjcs"), false);
}

TEST(EarleyAlgTest, AcceptWordTest2) {
  Earley earley;
  fit_for_test(earley.grammar, "2 3 5\nST\nabc\nS->T\nT ->\nT->aTa\nT->bTb\nT->cTc\nS");
  EXPECT_EQ(earley.Predict("aa"), true);
  EXPECT_EQ(earley.Predict("abaaba"), true);
  EXPECT_EQ(earley.Predict("bcbcaacbcb"), true);
}

TEST(EarleyAlgTest, RejectWordTest2) {
  Earley earley;
  fit_for_test(earley.grammar, "2 3 5\nST\nabc\nS->T\nT ->\nT->aTa\nT->bTb\nT->cTc\nS");
  EXPECT_EQ(earley.Predict("aabbba"), false);
  EXPECT_EQ(earley.Predict("bc"), false);
  EXPECT_EQ(earley.Predict("cdkjcs"), false);
}

TEST(EarleyAlgTest, AcceptWordTest3) {
  Earley earley;
  fit_for_test(earley.grammar, "3 3 6\nST F\nabc\nS->SaT\nS ->\nT->aTa\nT->bFb\nF->ccF\nF->\nS");
  EXPECT_EQ(earley.Predict("abb"), true);
  EXPECT_EQ(earley.Predict("abbaabccba"), true);
  EXPECT_EQ(earley.Predict(""), true);
}

TEST(EarleyAlgTest, RejectWordTest3) {
  Earley earley;
  fit_for_test(earley.grammar, "3 3 6\nST F\nabc\nS->SaT\nS ->\nT->aTa\nT->bFb\nF->ccF\nF->\nS");
  EXPECT_EQ(earley.Predict("ccccac"), false);
  EXPECT_EQ(earley.Predict("abbc"), false);
  EXPECT_EQ(earley.Predict("abbaabcchba"), false);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}