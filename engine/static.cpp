#include<iostream>
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<time.h>
#include <unistd.h>
#include"../eval.h"
#include"../othero.h"
#include"../learn.h"
#include"./one.h"
#include"./static.h"
#include"../env.h"
#include<fstream>
using namespace std;

//一手読み
//te 先手，後手
void Static::one(int te)
{
  //手順：　先手で
  //指す->入力される->合法手判断->適用(再入力)->繰り返し
  int i,count=1;
  int gyo;
  char retu;
  uint64_t pos,legal;
  BitBoard board;
  //Eval eval;
  Othero::init(&board);
  Othero::show(&board);
  cout << "試合開始" << endl;
  fflush(stdout);
  //eval.openeval();
  //対戦部分(人間先手1，後手-1)
  if(te==-1)
  {
    while(Othero::checkGameover(&board)!=GAME_OVER&&count<100)
    {
      if (board.teban==SENTE)
      {
        //ai側
        cout << "sente" << endl;
        fflush(stdout);
        legal=Othero::canReverse(&board);
        if(!legal){
          board.teban = GOTE;
          //count++;
          continue;
        }
        else
        {
          pos = this->evalPos(legal,board);
          //cout << "evalPos=" << pos << endl;
          if (!pos) return;
          Othero::put(pos,&board);
        }
        Othero::show(&board);
        board.teban=GOTE;
      }
      else
      {
        //人側
        cout << "gote" << endl;
        fflush(stdout);
        pos = Othero::canReverse(&board) & Othero::inputPos();
        legal=Othero::canReverse(&board);
        if(!legal){
          board.teban = SENTE;
          //count++;
          continue;
        }
        if(!pos)
        {
          continue;
        }

        Othero::put(pos,&board);
        Othero::show(&board);
        board.teban = SENTE;
      }

      count++;
      cout << count << "手" << endl;
    }
  }
  else if(te==1)//(te=1)の処理
  {
    while(Othero::checkGameover(&board)!=GAME_OVER&&count<100)
    {
      if (board.teban==SENTE)
      {
        //人側
        cout << "sente" << endl;
        fflush(stdout);
        pos = Othero::canReverse(&board) & Othero::inputPos();
        legal=Othero::canReverse(&board);
        if(!legal){
          board.teban = GOTE;
          //count++;
          continue;
        }
        if(!pos)
        {
          continue;
        }
  
        Othero::put(pos,&board);
        Othero::show(&board);
        board.teban = GOTE;
      }
      else
      {
        //ai側
        cout << "gote" << endl;
        fflush(stdout);
        legal=Othero::canReverse(&board);
        if(!legal){
          board.teban = SENTE;
          //count++;
          continue;
        }
        else
        {
          pos = this->evalPos(legal,board);
          //cout << "evalPos=" << pos << endl;
          if (!pos) return;
          Othero::put(pos,&board);
        }
        Othero::show(&board);
        board.teban=SENTE;
      }

  
      count++;
      cout << count << "手" << endl;
    }
  }
  else if(te==99)
  {
    
    while(Othero::checkGameover(&board)!=GAME_OVER&&count<100)
    {      
      if (board.teban==SENTE)
      {
        //ai側
        cout << "sente" << endl;
        fflush(stdout);
        legal=Othero::canReverse(&board);
        if(!legal){
          board.teban = GOTE;
          //count++;
          continue;
        }
        else
        {
          pos = this->evalPos(legal,board);
          if (!pos) return;
          Othero::put(pos,&board);
        }
        Othero::show(&board);
        board.teban=GOTE;
      }
      else
      {
        //ai側
        cout << "gote" << endl;
        fflush(stdout);
        legal=Othero::canReverse(&board);
        if(!legal){
          board.teban = SENTE;
          //count++;
          continue;
        }
        else
        {
          pos = this->evalPos(legal,board);
          if (!pos) return;
          Othero::put(pos,&board);
        }
        Othero::show(&board);
        board.teban=SENTE;
      }

  
      count++;
      cout << count << "手" << endl;
    }
  }
  
  cout << count << "手" << endl;
  Othero::result(&board);
  return;
}

string Static::woi(){
  return "静的評価関数";
  //return to_string(eval.dir4[0]);
}

uint64_t Static::go(){
  BitBoard tmp = this->getboard();
  uint64_t tmp2;
  tmp2 = this->bestPos(tmp);
  if(DEBUG_MODE) cout <<"bestpos "<< tmp2 << endl;
  return tmp2;
  // return true;
}

//現在の局面を評価し最善手を返す
uint64_t Static::bestPos(BitBoard board){
  BitBoard tmp = board;
  uint64_t legal = Othero::canReverse(&board);
  int legalnum = Othero::bitCount(legal);

  uint64_t pos = this->evalPos(legal,board);
  if(DEBUG_MODE) cout <<"bestpos "<< pos << endl;
  return pos;
}

//探索 最善手を返す
uint64_t Static::search(BitBoard board,int depth){
  int best = (-1)*INFINITY;
  uint64_t bestPos;
  int val = 0;
  BitBoard tmp = board;
  uint64_t legal = Othero::canReverse(&board);
  int legalnum = Othero::bitCount(legal);
  int count = 0;
  for(int i=0; i<64; i++)
  {
    if(legalnum<=count) break;
    else if((legal&((uint64_t)(1)<<i))!=0)
    {
      //val = eval(Othero::vput((uint64_t)(1) << i,&board),ENGINEDEPTH);
      if(best<val) {best = val; bestPos = (uint64_t)(1) << i;}
      count++;
    }
  }
  return bestPos;
}

//評価値
uint64_t Static::evalPos(uint64_t legal,BitBoard board){
  int best = (-1)*INFINITY;
  int tmpArray[64];
  BitBoard tmp = board;
  //uint64_t legal = Othero::canReverse(&board);
  int legalnum = Othero::bitCount(legal);
  int count = 0;
  int val;
  int index=0;
  //if(depth == 0){
  //}
  if(Othero::checkGameover(&board) == GAME_OVER){
  }
  if(legalnum == 0){
    //Othero::inverseTEBAN(&tmp);
    //this->evalPos(-1,tmp,depth-1);
    //Othero::inverseTEBAN(&tmp);
    //if(best<val) best = val;
    //return best;
    return -1;
  }

  for(int i=0; i<64; i++){
    val = 0;
    if(legalnum<=count) break;
    else if((legal&((uint64_t)(1)<<i))!=0){
      count++;
      //BitBoard temp = board;
      tmp = Othero::vput((uint64_t)(1)<<i,&board);
      tmp.teban = board.teban;
      Othero::inverseTEBAN(&tmp);
      this->bitboardToArray(tmp,tmpArray);
      //val = this->eval();
      for (int j = 0; j < 64; j++) {
        val += tmpArray[j] * this->staticBoard[j];
      }
      if(DEBUG_MODE) cout<< "now val:" << val <<" now best:" << best << endl;
      Othero::inverseTEBAN(&tmp);//手番をもとに戻す この時点で入力の手番と同じになる
      if(tmp.teban==GOTE)val *= -1;//後手番の場合評価値逆転
      if(DEBUG_MODE)cout << "現在の評価値" << val << endl;
      if(best<val) {best = val;index=i;if(DEBUG_MODE)cout<<index<<endl;}
    }
  }
  if(DEBUG_MODE)cout << "index :" << index << " value:" << ((uint64_t)(1)<<index) <<endl;
  return (uint64_t)(1)<<index;
}

void Static::bitboardToArray(BitBoard board,int* array){
  for (int i = 0; i < 64; i++) {
    if((board.black&(uint64_t(1)<<i))!=0){
      array[63-i] = 1;
    }
    else if((board.white&(uint64_t(1)<<i))!=0){
      array[63-i] = -1;
    }
    else{
      array[63-i] = 0;
    }
  }
  if(DEBUG_MODE){
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        cout << array[i*8+j];
      }
      cout << endl;
    }
    cout << "end" << endl;
  }
  return ;
}

void Static::openEval(){
  std::ifstream ifs("./staticEvalBoard1.txt");
  if(!ifs){
    if(DEBUG_MODE)std::cout << "ファイルが開けませんでした．" << endl;
    return;
  }
  if(DEBUG_MODE)std::cout << "ファイルを開けたよ" << endl;
  for (int i = 0; i < 64; i++) {
    ifs >> this->staticBoard[i];
  }
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      if(DEBUG_MODE)std::cout << " " <<this->staticBoard[i*8+j];
    }
    std::cout << std::endl;
    
  }
  return;
}


Static::Static(){
  this->openEval();
}

Static::~Static(){

}
