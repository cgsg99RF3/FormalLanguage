#include <gtest/gtest.h>
#include <stdexcept>
#include "../src/lr1.h"

TEST(LR1AlgTest, InvalidInput) {
  LR1_Parser lr_1_parser;
  EXPECT_ANY_THROW(fit_for_test(lr_1_parser.grammar, "1 3 1\nS\naSB\nS->abS\nS"));
  EXPECT_ANY_THROW(fit_for_test(lr_1_parser.grammar, "1 1 1\nS\nab\nS->abS\nS"));
  EXPECT_ANY_THROW(fit_for_test(lr_1_parser.grammar, "1 2 1\nS\nab\nS->abS\nf"));
}

TEST(LR1ParserAlgTest, AcceptWordTest1) {
  LR1_Parser lr_1_parser;
  fit_for_test(lr_1_parser.grammar, "1 2 2\nS\nab\nS->aSbS\nS ->\nS");
  lr_1_parser.ConstructParser();
  EXPECT_EQ(lr_1_parser.Predict("aababb"), true);
  EXPECT_EQ(lr_1_parser.Predict("ab"), true);
  EXPECT_EQ(lr_1_parser.Predict(""), true);
  EXPECT_EQ(lr_1_parser.Predict("abababab"), true);
}

TEST(LR1ParserAlgTest, RejectWordTest1) {
  LR1_Parser lr_1_parser;
  fit_for_test(lr_1_parser.grammar, "1 2 2\nS\nab\nS->aSbS\nS ->\nS");
  lr_1_parser.ConstructParser();
  EXPECT_EQ(lr_1_parser.Predict("aabbba"), false);
  EXPECT_EQ(lr_1_parser.Predict("bbb"), false);
  EXPECT_EQ(lr_1_parser.Predict("cdkjcs"), false);
}

TEST(LR1ParserAlgTest, AcceptWordTest2) {
  LR1_Parser lr_1_parser;

  fit_for_test(lr_1_parser.grammar, "2 3 5\nST\nabc\nS->T\nT ->\nT->aTa\nT->bTb\nT->cTc\nS");
  lr_1_parser.ConstructParser();
  EXPECT_EQ(lr_1_parser.Predict("aa"), true);
  EXPECT_EQ(lr_1_parser.Predict("abaaba"), true);
  EXPECT_EQ(lr_1_parser.Predict("bcbcaacbcb"), true);
}

TEST(LR1ParserAlgTest, RejectWordTest2) {
  LR1_Parser lr_1_parser;
  fit_for_test(lr_1_parser.grammar, "2 3 5\nST\nabc\nS->T\nT ->\nT->aTa\nT->bTb\nT->cTc\nS");
  lr_1_parser.ConstructParser();
  EXPECT_EQ(lr_1_parser.Predict("aabbba"), false);
  EXPECT_EQ(lr_1_parser.Predict("bc"), false);
  EXPECT_EQ(lr_1_parser.Predict("cdkjcs"), false);
}

TEST(LR1ParserAlgTest, AcceptWordTest3) {
  LR1_Parser lr_1_parser;

  fit_for_test(lr_1_parser.grammar, "3 3 6\nST F\nabc\nS->SaT\nS ->\nT->aTa\nT->bFb\nF->ccF\nF->\nS");
  lr_1_parser.ConstructParser();
  EXPECT_EQ(lr_1_parser.Predict("abb"), true);
  EXPECT_EQ(lr_1_parser.Predict("abbaabccba"), true);
  EXPECT_EQ(lr_1_parser.Predict(""), true);
}

TEST(LR1ParserAlgTest, RejectWordTest3) {
  LR1_Parser lr_1_parser;
  fit_for_test(lr_1_parser.grammar, "3 3 6\nST F\nabc\nS->SaT\nS ->\nT->aTa\nT->bFb\nF->ccF\nF->\nS");
  lr_1_parser.ConstructParser();
  EXPECT_EQ(lr_1_parser.Predict("ccccac"), false);
  EXPECT_EQ(lr_1_parser.Predict("abbc"), false);
  EXPECT_EQ(lr_1_parser.Predict("abbaabcchba"), false);
}

TEST(LR1ParserAlgTest, AcceptWordTest4) {
  LR1_Parser lr_1_parser;

  fit_for_test(lr_1_parser.grammar, "3 5 6\nGET\nftabc\nG->EaE\nG->f\nE->T\nE->EbT\nT->f\nT->Tcf\nG");
  lr_1_parser.ConstructParser();
  EXPECT_EQ(lr_1_parser.Predict("faf"), true);
  EXPECT_EQ(lr_1_parser.Predict("fcfbfcfafbf"), true);
  EXPECT_EQ(lr_1_parser.Predict("f"), true);
}

TEST(LR1ParserAlgTest, RejectWordTest4) {
  LR1_Parser lr_1_parser;
  fit_for_test(lr_1_parser.grammar, "3 5 6\nGET\nftabc\nG->EaE\nG->f\nE->T\nE->EbT\nT->f\nT->Tcf\nG");
  lr_1_parser.ConstructParser();
  EXPECT_EQ(lr_1_parser.Predict("ccccac"), false);
  EXPECT_EQ(lr_1_parser.Predict("fafd"), false);
  EXPECT_EQ(lr_1_parser.Predict("fcfbbfcf"), false);
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}