#ifndef INC_DISTANCE_H_
#define INC_DISTANCE_H_

extern int StartTimeUpEdge_Rear;
extern int EndTimeDownEdge_Rear;
extern int StartTimeUpEdge_Front;
extern int EndTimeDownEdge_Front;
extern float distance_rear;
extern float distance_front;
extern char message_rear[20];
extern char message_front[20];

//倒车报警
void Distance_Rear(float Distance);
//前后测距
void Fore_Rea_Ranging();
void Fore_Rea_Ranging_Fast();

#endif /* INC_DISTANCE_H_ */
