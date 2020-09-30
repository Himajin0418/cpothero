#include<stdio.h>
#include<stdlib.h>
#include "./eval.h"
#include "./othero.h"
#include "./learn.h"
#include "./solve.h"
#include "./engine/one.h"

//うまく行ってる？
//どこに打てばいいかと，最終予想石差
//最終一手最適化をしよう

Solve::Solve()
{
    Othero othero;
}
int Solve::solver(BitBoard board){

  int i,j;
  int val;
  int legalnum;
  int count=0;
  int best = (-1)*INFINITY;
  BitBoard temp;
  uint64_t legal = Othero::canReverse(&board);
  legalnum=Othero::bitCount(legal);

  //DEBUG
  //printf("TEBAN=%d\n",board.teban);
  //show(&board);

  /* 葉の場合、評価値を返す */
  if(Othero::checkGameover(&board)==GAME_OVER)
  {
    //if(board.teban == SENTE)//後手が最後に指した
    if(board.teban == GOTE)//後手が最後に指した
    {
      return Othero::bitCount(board.white) - Othero::bitCount(board.black);
    }
    else//先手が最後に指した
    {
      return Othero::bitCount(board.black) - Othero::bitCount(board.white);
    }
  } //return eval();

  /* 現在の局面から1手進めた状態をa1,a2,a3・・akとする */
  //expand_node(node, 次の turn);


  //pass
  if(legalnum == 0)
  {
    temp = board;
    Othero::inverseTEBAN(&temp);
    //put(1<<i,&temp);
    val = (-1)*solver(temp);
    if(best<val) best = val;
    return best;
  }
  
  //
  for(i=0; i<64; i++)
  {
    if(legalnum<=count) break;
    else if((legal&((uint64_t)(1)<<i))!=0)
    {
      count++;
      //BitBoard temp = board;
      temp = vput((uint64_t)(1)<<i,&board);
      temp.teban = board.teban;
      Othero::inverseTEBAN(&temp);
      val = (-1)*solver(temp);
      if(best<val) best = val;
    }
    //val = - Negamax(ai, 次の turn, depth-1);
    //if(best < val)   best= val;
  }

  return best;
}

int Solve::solverMM(BitBoard board){

  int i,j;
  int val;
  int legalnum;
  int count=0;
  int best = (board.teban)*INFINITY;
  BitBoard temp;
  uint64_t legal = Othero::canReverse(&board);
  legalnum=Othero::bitCount(legal);

  //DEBUG
  //printf("TEBAN=%d\n",board.teban);
  //show(&board);

  /* 葉の場合、評価値を返す */
  if(Othero::checkGameover(&board)==GAME_OVER)
  {
    return Othero::bitCount(board.black) - Othero::bitCount(board.white);
  } //return eval();

  /* 現在の局面から1手進めた状態をa1,a2,a3・・akとする */
  //expand_node(node, 次の turn);


  //pass
  if(legalnum == 0)
  {
    temp = board;
    //temp.teban = (-1)*board.teban;
    Othero::inverseTEBAN(&temp);
    //put(1<<i,&temp);
    val = solverMM(temp);
    //if(best<val) best = val;
    return val;
  }
  
  //
  if(board.teban==SENTE)
  {
    for(i=0; i<64; i++)
    {
      if(legalnum<=count) break;
      else if((legal&((uint64_t)1<<i))!=0)
      {
        count++;
        //BitBoard temp = board;
        temp = vput((uint64_t)1<<i,&board);
        //temp.teban = board.teban;
        //temp.teban = (-1)*board.teban;
        Othero::inverseTEBAN(&temp);
        val = solverMM(temp);
        if(best<val) best = val;
      }
      //val = - Negamax(ai, 次の turn, depth-1);
      //if(best < val)   best= val;
    }
  }
  else
  {
    for(i=0; i<64; i++)
    {
      if(legalnum<=count) break;
      else if((legal&((uint64_t)1<<i))!=0)
      {
        count++;
        //BitBoard temp = board;
        temp = vput((uint64_t)1<<i,&board);
        //temp.teban = (-1)*board.teban;
        Othero::inverseTEBAN(&temp);
        val = solverMM(temp);
        if(best>val) best = val;
      }
      //val = - Negamax(ai, 次の turn, depth-1);
      //if(best < val)   best= val;
    }
  }

  return best;
}


/*
void inverseTEBAN(BitBoard *board)
{
  if(board->teban == SENTE) board->teban = GOTE;
  else board->teban = SENTE;
  return ;
}
*/
