


void tim_ini(void);

////////////////////////////////////////////////
void tim_ini(void)
{
  TIM_TimeBaseInitTypeDef TIMER_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

  TIM_TimeBaseStructInit(&TIMER_InitStructure);
  TIMER_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIMER_InitStructure.TIM_Prescaler = 20;
  TIMER_InitStructure.TIM_Period = 73;
  TIMER_InitStructure.TIM_Prescaler = 2;
  TIMER_InitStructure.TIM_Period = 2;
  TIM_TimeBaseInit(TIM4, &TIMER_InitStructure);
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
  TIM_Cmd(TIM4, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
/////////////////////////////////////////////////
int shet_taktov = 0;
long chislo = 0;
int read_bit = 0;
int chet = 0;
int takt = 0;
float priveden_znach = 0.0;
float viborka[1000];
int schet = 0;
int flag = 0;
int N = 1000;
///////////////////////////////////////////////
void TIM4_IRQHandler(void)
{
  if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {

      TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
      if(shet_taktov == 0)
        {
          GPIO_ResetBits(GPIOC, GPIO_Pin_13);  // опускае CS
        }
      else if(shet_taktov > 0)
        {
          GPIOE->ODR ^= GPIO_Pin_4;     // каждый раз одрим ножку тем самым делая клоки
          if(chet == 0)
            {
              if(takt < 18) // пока не считаем 18 бит
                {
                  read_bit = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_6);  // читаем  бит
                  chislo = chislo << 1;  // сдвигаем старое значение
                  chislo = chislo | read_bit; //  добавляем к старому значению новый бит
                  chet++;
                  takt++;
                }
            }
          else if(chet == 1)
            {
	          chet = 0;
		    }

        }
      shet_taktov++;
      if(shet_taktov > 38)
	    {
		  GPIO_SetBits(GPIOC, GPIO_Pin_13);   // подымаем CS
		}
      if(shet_taktov > 42)   // если чтение одного значения закончилось
        {
          if(chislo > 131072) // проверка на больше меньше нуля
            {
              chislo = chislo - 262144;
            }
          else if(chislo <= 131072)
            {
              chislo = chislo;
            }
          priveden_znach = chislo * 0.00003052;  // домнажаем на мВ
          if(flag == 0)   
            {
              viborka[schet] = priveden_znach;  // записываем значение в масив
              priveden_znach = 0;
              schet++;   // плюсуем счетчик выборки
            }
          if(schet == N)  // если набрали нужную выборку
            {
              flag = 1;
              GPIO_SetBits(GPIOE, GPIO_Pin_4);  // подымаем клоки
              TIM_Cmd(TIM4, DISABLE);           // вырубаем таймер чтобы спокойно делать математику
            }
          shet_taktov = 0;
          takt = 0;
          chet = 0;
          chislo = 0;
          read_bit = 0;
          
        }
      
    }
}