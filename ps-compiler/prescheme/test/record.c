#include <stdio.h>
#include "prescheme.h"

struct pair {
  long car;
  struct pair *cdr;
};
struct pair *cons(unsigned char, struct pair*);
struct pair *reverseB(struct pair*);
char memberP(struct pair*, long);
void init(void);
struct pair *null;

struct pair *cons(unsigned char car_0X, struct pair *cdr_1X)
{
  struct pair *pair_2X;
 {pair_2X = (struct pair)malloc(sizeof(struct pair));
  if ((NULL == pair_2X)) {
    return pair_2X;}
  else {
    pair_2X->car = (((unsigned long) car_0X));
    pair_2X->cdr = cdr_1X;
    return pair_2X;}}}

struct pair *reverseB(struct pair *list_3X)
{
  struct pair *arg0K1;
  struct pair *arg0K0;
  struct pair *list_4X;
  struct pair *prev_5X;
  struct pair *next_6X;
 {if ((list_3X == (null))) {
    return list_3X;}
  else {
    if (((list_3X->cdr) == (null))) {
      return list_3X;}
    else {
      arg0K0 = list_3X;
      arg0K1 = (null);
      goto L105;}}}
 L105: {
  list_4X = arg0K0;
  prev_5X = arg0K1;
  next_6X = list_4X->cdr;
  list_4X->cdr = prev_5X;
  if ((next_6X == (null))) {
    return list_4X;}
  else {
    arg0K0 = next_6X;
    arg0K1 = list_4X;
    goto L105;}}}

char memberP(struct pair *list_7X, long x_8X)
{
  struct pair *arg0K0;
  struct pair *list_9X;
 {arg0K0 = list_7X;
  goto L169;}
 L169: {
  list_9X = arg0K0;
  if ((list_9X == (null))) {
    return 0;}
  else {
    if ((x_8X == (list_9X->car))) {
      return 1;}
    else {
      arg0K0 = (list_9X->cdr);
      goto L169;}}}}

void init(void)
{
  struct pair *arg0K0;
  struct pair *pair_10X;
  struct pair *cdr_11X;
  struct pair *v_12X;
 {cdr_11X = null;
  pair_10X = (struct pair)malloc(sizeof(struct pair));
  if ((NULL == pair_10X)) {
    arg0K0 = pair_10X;
    goto L199;}
  else {
    pair_10X->car = 0;
    pair_10X->cdr = cdr_11X;
    arg0K0 = pair_10X;
    goto L199;}}
 L199: {
  v_12X = arg0K0;
  null = v_12X;
  return;}}
void
main(void)
{
}
