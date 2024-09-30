
#include "bt_config.h"
#include "bt_os.h"
#include "bt_api.h"
#include "bt_core.h"


#ifdef __cplusplus
extern "C" {
#endif

u8 	phone_macaddr_buf[12]={'\0'};//�ֻ�����MAC ADDR
u8 	phone_name_buf[24]={'\0'};//�ֻ���������
u8 	bt_hostname_buf[24]={'\0'};//������������
u8 	bt_pincard_buf[4]={'\0'};//���������
u8 	bt_macaddr_buf[12]={'\0'};//��������MAC ADDR
u8	bt_callin_buf[64];//�ֻ��������

bt_phone_state_t	bt_phone_state; //�ֻ�ͨ��״̬
bt_phone_type_t		bt_phone_type;   //�ֻ���������
bt_connect_flag_t 	bt_connect_flag = BT_STATE_INIT;
bt_spp_connect_flag_t 	bt_spp_connect_flag = BT_SPP_STATE_INIT;
bt_ble_connect_flag_t 	bt_ble_connect_flag = BT_BLE_STATE_INIT;
bt_time_t           bt_time;//����ʱ��

bt_phonebook_state		        *bt_phonebook_para = NULL;
bt_ID3_info_t				    *bt_ID3_info = NULL;


//fmrx
u16                 ChtoFreq = 0;//���ؽ�Ŀ�Ŷ�Ӧ��Ƶ��
bt_fmrx_info_t		bt_fmrx_info;//ĳ����Ŀ����Ϣ�����ܽ�Ŀ������ǰ��Ŀ�ţ��Լ���Ӧ��Ƶ��
bt_fmrx_list_t		bt_fmrx_list;//��Ŀ��Ƶ���б�



//����ʱ����Ҫ�������32.768K����
 __s32 sys_bt_set_time(u8* time)
{	
	#if 0
	__u8 i;
	__msg("date time:");
	for(i=0;i<10;i++)
	{
		printf("%d ",time[i]);
	}
	printf("\n");
	#endif
	bt_time.year=time[0];
	bt_time.month=time[1];
	bt_time.day=time[2];
	bt_time.hour=time[3];
	bt_time.minute=time[4];
	
	sys_bt_get_time_to_app(&bt_time);
	
	return EPDK_OK;
}

__s32 sys_bt_get_time_to_app(bt_time_t *time)
{
#if RTOS	
	//�����ǰ�BT��ȡ��ʱ�����ø�GUIָʾʱ��
	{
		tui_time_t tui_time;
		tui_time.year = bt_time.year+2000;
		tui_time.mon = bt_time.month;
		tui_time.mday = bt_time.day;
		tui_time.hour = bt_time.hour;
		tui_time.min= bt_time.minute;				
		tui_time.sec= 0;
		//printf("set time: year =%d,mday =%d,mday =%d",tui_time.year,tui_time.mon,tui_time.mday);
		tui_set_localtime(&tui_time);
	}
#endif	
    return EPDK_OK;
}
//appע��year�任���������ȼ�2000
__s32 sys_bt_set_time_to_bt(bt_time_t *time)
{
    bt_time.year = time->year;
    bt_time.month= time->month;
    bt_time.day=  time->day;
    bt_time.hour= time->hour;
    bt_time.minute= time->minute;
    printf(" bt year =%d,mon=%d,day=%d,hour=%d,min=%d",bt_time.year,bt_time.month,bt_time.day,bt_time.hour,bt_time.minute);
    bt_time_set();
    return EPDK_OK;
}


 void sys_bt_ctrl_lcd(u8 lcd_ctrl)
{
#if RTOS
	if(lcd_ctrl == '0')//�ػ�����
	{	
		tui_sys_msg_send(TUI_DSK_MSG_SYS_POWER_OFF, NULL, NULL);	
	}
	else if(lcd_ctrl =='1')//��������
	{	
		tui_sys_msg_send(TUI_DSK_MSG_SYS_POWER_ON, NULL, NULL);
	}
#endif		
	return;
}

void sys_bt_ctrl_light(u8 light_ctrl)
{
#if RTOS
    if(light_ctrl=='0')///��ƹ�
    {
    	//printf("==========big led close=========\n");

       system_set_ill_state(ILL_STAT_OFF);
      /*
    	turn_off_red();
    	turn_off_green();
    	turn_off_blue();
      */
      
      tui_sys_msg_send(TUI_DSK_MSG_SYS_BIG_LED, NULL, NULL);       
    }
	else if(light_ctrl=='1')///��ƿ�
	{
		//printf("===========big led open=========\n");
		system_set_ill_state(ILL_STAT_ON);
		
       /*
		show_light_state();
	   */
	   
	   //����ֱ�ӵ���
	   //dsk_display_set_lcd_brightness(LION_BRIGHT_LEVEL3);	   
	   tui_sys_msg_send(TUI_DSK_MSG_SYS_BIG_LED, NULL, NULL);		
	}
#endif	
	return;
}
 void sys_bt_ctrl_pev(u8 pev_ctrl)
{
#if RTOS
    __msg("=====pev:%x====\n",pev_ctrl);
    if(pev_ctrl=='1')///pev  1->0  2->1
    {
    	__msg("=================prv enter==============\n");
    	if(system_get_cur_app()==ID_APP_PEV)
    	{
    	    return ;
    	}
    	
    	tui_sys_msg_send(TUI_DSK_MSG_CAR_PEV_ON, NULL, NULL);
    }
    else if(pev_ctrl=='0')///exit pev
    {
    	__msg("=================prv exit==============\n");
    	tui_sys_msg_send(TUI_DSK_MSG_CAR_PEV_OFF, NULL, NULL);
    }
#endif
    return;
}
__s32 sys_bt_set_phonebook(u8 *phonebook)
{///bt_phonebook_para
	__s32 namelen = 0;////���Ƴ���
	__s32 numlen = 0;////�绰���볤��
	__s32 datelen = 0;////���ڳ���
	__u8  type=0;
	
	__s32 numstart_index = 0;
	__s32 i=0;
	__s32 second_num_index=0;
	__s32 second_num_cnt=0;
	__s32 second_num_flag=0;
	__u8  *ptr=0;
	
//	__msg("phonebook:[%s]\n",system_do_msg);
	///ȡ���Ƴ���
	namelen=phonebook[1];
	numlen=phonebook[2];
	datelen=phonebook[3];
	type=phonebook[4];
	
	////ָ��NAME
	ptr=&(phonebook[5]);
	//__wrn("namelen =%d,numlen=%d,datelen =%d,type=%d",namelen,numlen,datelen,type);
	__wrn("ptr =%s",ptr);
	switch(type)
	{
		case 0x0://�绰��
		{

		   strncpy(bt_phonebook_para->phonebook_name[bt_phonebook_para->cur_phone_total], ptr, namelen+1);
	      bt_phonebook_para->phonebook_name[bt_phonebook_para->cur_phone_total][namelen]='\0';
	       strncpy(bt_phonebook_para->phonebook_number[bt_phonebook_para->cur_phone_total], ptr+namelen, numlen);
          //������������ֻ��ȡһ������
	      // strncpy(bt_phonebook_para->phonebook_number[bt_phonebook_para->cur_phone_total], ptr+namelen, 11);
		  bt_phonebook_para->phonebook_number[bt_phonebook_para->cur_phone_total][numlen]='\0';
		  //__wrn("phonebook_name =%s",bt_phonebook_para->phonebook_name[bt_phonebook_para->cur_phone_total]);
		  //__wrn("phonebook_number =%s",bt_phonebook_para->phonebook_number[bt_phonebook_para->cur_phone_total]);
		  bt_phonebook_para->cur_phone_total++;
		  __wrn("cur_phone_total =%d",bt_phonebook_para->cur_phone_total);
		  if(bt_phonebook_para->cur_phone_total>=999)
		   bt_phonebook_para->cur_phone_total= 999;
		 
		  break;
		}
		case 0x1://�������
		{

			 strncpy(bt_phonebook_para->phoneout_name[bt_phonebook_para->cur_phoneout_total], ptr, namelen+1);
	        bt_phonebook_para->phoneout_name[bt_phonebook_para->cur_phoneout_total][namelen]='\0';
	         strncpy(bt_phonebook_para->phoneout_number[bt_phonebook_para->cur_phoneout_total], ptr+namelen, numlen);
		    bt_phonebook_para->phoneout_number[bt_phonebook_para->cur_phoneout_total][numlen]='\0';
		    //__wrn("phoneout_number =%s",bt_phonebook_para->phoneout_name[bt_phonebook_para->cur_phoneout_total]);
		    //__wrn("phoneout_number =%s",bt_phonebook_para->phoneout_number[bt_phonebook_para->cur_phoneout_total]);
		    bt_phonebook_para->cur_phoneout_total++;
			break;
		}
		case 0x2://�������
		{
			 strncpy(bt_phonebook_para->phonein_name[bt_phonebook_para->cur_phonein_total], ptr, namelen+1);
	        bt_phonebook_para->phonein_name[bt_phonebook_para->cur_phonein_total][namelen]='\0';
	         strncpy(bt_phonebook_para->phonein_number[bt_phonebook_para->cur_phonein_total], ptr+namelen, numlen);
		    bt_phonebook_para->phonein_number[bt_phonebook_para->cur_phonein_total][numlen]='\0';
		    //__wrn("phonein_number =%s",bt_phonebook_para->phonein_name[bt_phonebook_para->cur_phonein_total]);
		    //__wrn("phonein_number =%s",bt_phonebook_para->phonein_number[bt_phonebook_para->cur_phonein_total]);
		    bt_phonebook_para->cur_phonein_total++;
			break;
		}
		case 0x3://δ�Ӻ���
		{
			 strncpy(bt_phonebook_para->phonemiss_name[bt_phonebook_para->cur_phonemiss_total], ptr, namelen+1);
	        bt_phonebook_para->phonemiss_name[bt_phonebook_para->cur_phonemiss_total][namelen]='\0';
	         strncpy(bt_phonebook_para->phonemiss_number[bt_phonebook_para->cur_phonemiss_total], ptr+namelen, numlen);
		    bt_phonebook_para->phonemiss_number[bt_phonebook_para->cur_phonemiss_total][numlen]='\0';
		    //__wrn("phonemiss_number =%s",bt_phonebook_para->phonemiss_name[bt_phonebook_para->cur_phonemiss_total]);
		    //__wrn("phonemiss_number =%s",bt_phonebook_para->phonemiss_number[bt_phonebook_para->cur_phonemiss_total]);
		    bt_phonebook_para->cur_phonemiss_total++;
			break;
		}
		case 0xf0: //�������,�û����ͱ�ǵ�GUI��ʾ
		{
#if RTOS	
			tui_sys_msg_send(TUI_USER_MSG_APP_GO_BT_BOOK_SHOW, NULL, NULL);
#endif			
			break;
		}
		default:
		{
		   __wrn("not find cmd");
		   break;
		}
	}
	return 0;
}

//��ȡ����ָʾ�绰����
__s32  sys_bt_get_phonenum(char *btstring)
{
	__s32 i=0;
	char *p = &(btstring[2]);
	
	if(system_do_msg[1]!='G')
	{
		__msg("=====Wrong phone number====\n");
		return 0;
	}
	memset(bt_callin_buf, 0, 64);
	
	for(i=0;i<60;i++,p++)
	{
		if(*p=='G')
		{
			__inf("\n==='G'over===\n");
			break;
		}
		bt_callin_buf[i] = *p;////sys_docmd[i+2];
		//__inf("=%x=",bt_callin_buf[i]);
	}
	//__inf("==i:%d==\n",i);
	bt_callin_buf[i]='\0';
	__msg("======phone number=%s=%d=====\n",bt_callin_buf,strlen(bt_callin_buf));
	//__msg("=====Telephone number:%s======\n",bt_callin_buf);
	return 0;
}

//����绰����¼
void sys_bt_clear_phonebook(void)
{
#if 1//test bug died
    if(bt_phonebook_para != NULL)
    {
    	//bt_phonebook_para->cur_phone_total=0;
    	//bt_phonebook_para->cur_phoneout_total=0;
    	//bt_phonebook_para->cur_phonein_total=0;
    	//bt_phonebook_para->cur_phonemiss_total=0;
    	memset(bt_phonebook_para,0,sizeof(bt_phonebook_state));   	
	}
#endif	
}
__s32 sys_bt_fm_process(void)
{
    switch(system_do_msg[1])
    {
        case 'A'://���ؽ�Ŀ�Ŷ�Ӧ��Ƶ��
        {            
             ChtoFreq = (system_do_msg[3]<<8)+system_do_msg[2];
        }
        break;
        case 'I':
        {   
            bt_fmrx_info.ch_cnt = system_do_msg[2];
            bt_fmrx_list.ch_cnt =bt_fmrx_info.ch_cnt;
            bt_fmrx_info.ch_cur = system_do_msg[3];
            bt_fmrx_info.freq = system_do_msg[5]<<8+system_do_msg[4];

        }
        break;
        case 'L':
        {
            u8 i;                
            bt_fmrx_list.ch_cnt = system_do_msg[2]; //һ��û��64����Ŀ  
            bt_fmrx_info.ch_cnt = bt_fmrx_list.ch_cnt;
            for(i = 0;i<bt_fmrx_list.ch_cnt;i++)
            {
                 //��0������ֵ���ǵ�һ����Ŀ��Ƶ��
                 bt_fmrx_list.freq[i]=(system_do_msg[4+i*2]<<8)+system_do_msg[3+i*2];
            }
        }
        break;
        case 'T':
        {
              bt_fmrx_info.ch_cnt = system_do_msg[2];
              bt_fmrx_list.ch_cnt = bt_fmrx_info.ch_cnt;
        }
        break;
        default:
        {

        }
        break;
    }
}

__s32 sys_bt_dosomething(void)
{
	__s32 i = 0;

	switch(system_do_msg[1])
	{
		case 'A'://HFP��ǰ״ָ̬ʾ->û����
		{
			__msg("=====================BT_STATE_NOTCONNECTE=====================\n");
			///ϵͳû���ϲ��ܣ����Ϻ����ͼ��
			if(bt_connect_flag	!= BT_STATE_NOTCONNECTE)
			{
			    if(bt_phonebook_para!= NULL)
            	{
            		sys_bt_clear_phonebook();
            	}
			 	bt_connect_flag = BT_STATE_NOTCONNECTE;
				bt_phone_state = BT_PHONE_INIT;
				bt_audio_set_status(BT_STAT_IDLE);
			    memset(phone_name_buf,0,24);
#if RTOS			    
                tui_obj_set_hidden(TUI_OBJ(GLOBAL_BAR_GLOBAL_VIEW_IMAGE_BTN_7), 1); 
#endif                
			}
			//�����Ͽ�����Ҫ�˵���һ��Ӧ�õ�
#if RTOS				
			if(system_get_bt_need_exit()==1)
			{
			    system_set_bt_need_exit(0);		    
			    tui_sys_msg_send(TUI_DSK_MSG_RUN_PREV_AUDIO_APP, NULL, NULL);		    
			}
#endif				
		}
		break;
		case 'B'://HFP��ǰ״ָ̬ʾ->�����ˡ�
		{
			__msg("=====================BT_STATE_CONNECTED=====================\n");
			///ϵͳ���ϲ��ܣ����Ϻ����ͼ��	
			////draw not connect icon
			bt_connect_flag	= BT_STATE_CONNECTED;
			bt_phone_state = BT_PHONE_INIT;
			bt_audio_set_status(BT_STAT_IDLE);
#if RTOS			
            tui_obj_set_hidden(TUI_OBJ(GLOBAL_BAR_GLOBAL_VIEW_IMAGE_BTN_7), 0); 
#endif			 
		}
		break;
		case 'C'://����PIN��
		{
			__msg("======================bt PIN=====================\n");
			for(i=0;i<24;i++)
			{
				if(system_do_msg[2+i]==0xff||system_do_msg[2+i]==0x0)
				{
					bt_pincard_buf[i]=0x0;
					__inf("=====over=====");
					break;
				}
				bt_pincard_buf[i]=system_do_msg[2+i];
				//__inf("=%x=",bt_pincard_buf[i]);
				
			}
#if RTOS			
			__msg("======================bt PIN :%s======================\n",bt_pincard_buf);
			tui_sys_msg_send(TUI_USER_MSG_APP_GO_BT_SET_PIN, NULL, NULL);
#endif
			
		}
		break;
		case 'D'://�ֻ����ڰκ�
		{
			__msg("======================phone call out====================\n");
			if(sys_bt_get_phone_state()==0)//////2016-01-15
			{
				if(bt_connect_flag!=BT_STATE_CONNECTED)
				{
					///__msg("=====there was no connection==============\n");
					bt_connect_flag	= BT_STATE_CONNECTED;
#if RTOS					
					tui_obj_set_hidden(TUI_OBJ(GLOBAL_BAR_GLOBAL_VIEW_IMAGE_BTN_7), 0);
#endif					
				}
				bt_phone_state = BT_PHONE_OUTING;
#if RTOS				
				tui_sys_msg_send(TUI_DSK_MSG_PHONE_CALLING, NULL, NULL);
#endif				
			}
			else
			{
				///__msg("===========����ͨ��״̬===========\n");
			}
		}
		break;
		case 'E'://�绰�ѹҶ�
		{
			__msg("==========The phone has been hung up==========\n");
			if(bt_connect_flag!=BT_STATE_CONNECTED)
			{
				///__msg("=====there was no connection==============\n");
				bt_connect_flag	= BT_STATE_CONNECTED;
#if RTOS				
				tui_obj_set_hidden(TUI_OBJ(GLOBAL_BAR_GLOBAL_VIEW_IMAGE_BTN_7), 0);
#endif				
			}
			bt_phone_state = BT_PHONE_INIT;
#if RTOS			
			tui_sys_msg_send(TUI_DSK_MSG_BREAK_CALLING, NULL, NULL);
#endif			
		}
		break;
		case 'F'://///////����ָʾ///�޵绰����
		{
#if RTOS
			__msg("===================Incoming call===================\n");
			if(bt_connect_flag!=BT_STATE_CONNECTED)
			{
				///__msg("=====there was no connection==============\n");
				bt_connect_flag	= BT_STATE_CONNECTED;
				
				tui_obj_set_hidden(TUI_OBJ(GLOBAL_BAR_GLOBAL_VIEW_IMAGE_BTN_7), 0);
			
			}
			if(bt_phone_state != BT_PHONE_INING)
			{
				bt_phone_state = BT_PHONE_INING;
			}
				
			__msg("===================================================\n");
			if(system_get_lcd_state()==LCD_STAT_OFF)//��ǰ����״̬
			{
				__msg("=========bt_audio_to_phone=========\n");
				bt_audio_to_phone();				
				return 0;
			}
			else
			{

				if(system_get_cur_app()==ID_APP_PEV)
				{
					if(get_vol_mute_state() == 1)
					{
						///__msg("===========unMUTE==========\n");
						tui_sys_msg_send(TUI_USER_MSG_APP_SPINNER_GO_VOL_MUTE, NULL, NULL);
						dsk_aux_set_volume(dsk_aux_get_volume());
					}
					///��ͨ��
					///__msg("==============��ͨ��============\n");
					dsk_aux_select_channel(AUX_AUDIO_CHANNEL);
					system_enter_denoise();
					///__msg("================��������==============\n");
					return 0;
				}
				else
				{
				    __u8 cur_app= system_get_cur_app();
					if(cur_app != ID_APP_BT)
					{
						///__msg("=========================SYSTEM_MEDIA_SAVE_BREAKPOINT=====================\n");
						//1,���浱ǰӦ����Ϣ
						//2,�л�������Ӧ��
                            tui_sys_msg_send(TUI_DSK_MSG_SYSTEM_SAVE_BREAKPOINT, 0, NULL);
                            system_set_bt_need_exit(1);
							tui_sys_msg_send(TUI_USER_MSG_APP_GO_BT, 0, NULL);
						///__msg("=========================ID_APP_BT CREAT FINISHED=====================\n");
					}
					else
					{
						if(bt_connect_flag!=BT_STATE_CONNECTED)
            			{
            				///__msg("=====there was no connection==============\n");
            				bt_connect_flag	= BT_STATE_CONNECTED;
            				tui_obj_set_hidden(TUI_OBJ(GLOBAL_BAR_GLOBAL_VIEW_IMAGE_BTN_7), 0);
            			}
						bt_phone_state = BT_PHONE_INING;
						tui_sys_msg_send(TUI_DSK_MSG_PHONE_CALLING, NULL, NULL);
					}
				}
			}
#endif			
		}
		break;
		case 'G'://{"ID",	4},///////����ָʾ///����ָʾ(�绰���)ͬʱȡ���绰����
		{
#if RTOS			
			__msg("===================Incoming call indication=======================\n");
			if(bt_connect_flag!=BT_STATE_CONNECTED)
			{
			    ///__msg("=====there was no connection==============\n");
				bt_connect_flag	= BT_STATE_CONNECTED;
				tui_obj_set_hidden(TUI_OBJ(GLOBAL_BAR_GLOBAL_VIEW_IMAGE_BTN_7), 0);
			}
			if(sys_bt_get_phone_state()==0)
			{
				bt_phone_state = BT_PHONE_INING;
				tui_sys_msg_send(TUI_DSK_MSG_PHONE_CALLING, NULL, NULL);
			}
			else
			{
				__msg("========Repeated calls========\n");
				bt_phone_state = BT_PHONE_INING;
				sys_bt_get_phonenum(system_do_msg);
				///__msg("====��ǰ��BT====\n");
				tui_sys_msg_send(TUI_DSK_MSG_PHONE_CALLING, NULL, NULL);
			}
#endif			
		}
		break;
		case 'I'://{"IG",	4},///////�绰����
		{
#if RTOS		
			__msg("=============================The phone has been answered777777======================\n");
	        if(bt_connect_flag!=BT_STATE_CONNECTED)
			{
			    ///__msg("=====there was no connection==============\n");
				bt_connect_flag	= BT_STATE_CONNECTED;
				tui_obj_set_hidden(TUI_OBJ(GLOBAL_BAR_GLOBAL_VIEW_IMAGE_BTN_7), 0);
			}
			bt_connect_flag	= BT_STATE_CONNECTED;
			/////////////////////////////////////////////////////////////////////////////////
			
			if(bt_phone_state ==BT_PHONE_INING)
			{
				__msg("==================talking in===================\n");
				///bt_phone_incoming();
				bt_phone_state = BT_PHONE_TALKINING;
			}
			else if(bt_phone_state ==BT_PHONE_OUTING)
			{
				__msg("==================talking out===================\n");
				bt_phone_state = BT_PHONE_TALKOUTING;
			}
			if(system_get_cur_app()== ID_APP_BT)
			{
				tui_sys_msg_send(TUI_USER_MSG_APP_GO_BT_PHONENUMBER_SHOW, NULL, NULL);
			}
#endif			
		}
		break;
		case 'K'://�������ӵ��ֻ���
		{
#if RTOS		
			__s32	i=0;
			
			__msg("=============================phone bt name======================\n");
			for(i=0;i<24;i++)
			{
				if(system_do_msg[2+i]==0xff||system_do_msg[2+i]==0x0)
				{
					phone_name_buf[i]=0x0;
					__inf("=====over=====");
					break;
				}
				phone_name_buf[i]=system_do_msg[2+i];
				//__inf("=%x=",phone_name_buf[i]);
				
			}
			__msg("=============================phone bt name:%s======================\n",phone_name_buf);
			tui_sys_msg_send(TUI_USER_MSG_APP_GO_BT_SET_BTNAME, NULL, NULL);
#endif			
		}
		break;
		case 'M'://{"mb",	4},///////������
		{
#if RTOS		
			__msg("============================play music======================\n");
			//////////////////////////////�⿪��û��ͼ�������/////////////////////////
			bt_connect_flag	= BT_STATE_CONNECTED;
			/////////////////////////////////////////////////////////////////////////////////
			//bt_phone_music_state = 1;
			bt_phone_state = BT_PHONE_INIT;
			//bt_mute_flag=0;    ///������
#endif			
		}
		break;
		case 'N'://�����豸��
		{
#if RTOS		
			__msg("=============================host bt name======================\n");		
			for(i=0;i<24;i++)
			{
				if(system_do_msg[2+i]==0xff||system_do_msg[2+i]==0x0)
				{
					bt_hostname_buf[i]=0x0;
					__inf("=====over=====");
					break;
				}
				bt_hostname_buf[i]=system_do_msg[2+i];
				//__inf("=%x=",bt_hostname_buf[i]);
				
			}
            __msg("=============================host bt name:%s======================\n",bt_hostname_buf);
			//bt_phone_music_state = 1;
			tui_sys_msg_send(TUI_USER_MSG_APP_GO_BT_SET_HOSTNAME, NULL, NULL);
#endif			
		}
		break;
		case 'S'://ȡBT STATE
		{
#if RTOS		
			// BT_STATUS_CONNECTING,       /*�����ӣ�û�е绰�������ڻ*///40
    		//BT_STATUS_TAKEING_PHONE,    /*���ڵ绰*/
    		//BT_STATUS_PLAYING_MUSIC,    /*��������*/
			__msg("===============the bt state in current:%d===========\n",system_do_msg[2]);
			if(system_do_msg[2]==40)//BT_STATUS_CONNECTING
			{
				if(bt_connect_flag!=BT_STATE_CONNECTED)
				{
					///__msg("======================֮ǰΪû������====================\n");
					bt_connect_flag	= BT_STATE_CONNECTED;
					tui_obj_set_hidden(TUI_OBJ(GLOBAL_BAR_GLOBAL_VIEW_IMAGE_BTN_7), 0);
				}
			}
			else if(system_do_msg[2]==41)//BT_STATUS_TAKEING_PHONE
			{
				///__msg("======================����ָʾ====================\n");
				
				if(bt_phone_state != BT_PHONE_INING)
				{
					bt_phone_state=BT_PHONE_INING;
				}
				///__msg("=============================����======================\n");
				if(system_get_lcd_state()==LCD_STAT_OFF)//��ǰ����״̬
				{
					///__msg("==========�е��ֻ���=========\n");
					bt_audio_to_phone();
					//bt_state=BT_PHONE_INING;
					return 0;
				}
				else
				{
					if(system_get_cur_app()==ID_APP_PEV)//�����ǰ�ǵ���Ӧ��
					{
						if(get_vol_mute_state() == 1)
						{
							///__msg("============��MUTE==========\n");
							tui_sys_msg_send(TUI_USER_MSG_APP_SPINNER_GO_VOL_MUTE, NULL, NULL);
							dsk_aux_set_volume(dsk_aux_get_volume());
						}
						///��ͨ��
						///__msg("==============��ͨ��============\n");
						dsk_aux_select_channel(AUX_AUDIO_CHANNEL);
						system_enter_denoise();
						return 0;
					}
					else
					{
					
						__u8 cur_app= system_get_cur_app();
					    if(cur_app != ID_APP_BT)
						{
							__msg("=========================SYSTEM_MEDIA_SAVE_BREAKPOINT=====================\n");
							//1,���浱ǰӦ����Ϣ
							//2,�л�������Ӧ��
                            tui_sys_msg_send(TUI_DSK_MSG_SYSTEM_SAVE_BREAKPOINT, NULL, NULL);
                            system_set_bt_need_exit(1);
							tui_sys_msg_send(TUI_USER_MSG_APP_GO_BT, 0, NULL);
						    __msg("=========================ID_APP_BT CREAT FINISHED=====================\n");
						}
						else
						{

							///__msg("====��ǰ��BT====\n");
							bt_phone_state = BT_PHONE_INING;
							__msg("=====show phone call in number:%s=====\n",bt_callin_buf);
							tui_sys_msg_send(TUI_USER_MSG_APP_GO_BT_PHONENUMBER_SHOW, NULL, NULL);
							
						}
					}
				
				}
			}
			else if(system_do_msg[2]==42)//BT_STATUS_PLAYING_MUSIC
			{
				
				bt_connect_flag	= BT_STATE_CONNECTED;
				bt_phone_state = BT_PHONE_INIT;
				
			}
#endif			
		}
		break;
		case 'H'://����MAC ADDR
		{
#if RTOS
			__msg("=============================mac addr======================\n");	
			char 	addr_buf[12]={0};
            char 	temp_buf[12]={0};
            
			for(i=0;i<24;i++)
			{
				if(system_do_msg[2+i]==0xff||system_do_msg[2+i]==0x0)
				{
					addr_buf[i]=0x0;
					__inf("=====over=====");
					break;
				}
			    addr_buf[i]=system_do_msg[2+i];
				if(addr_buf[i]<=0xf)
				{
				     sprintf(temp_buf, "0%x",addr_buf[i]);
				     strcat(bt_macaddr_buf, temp_buf);
				}
				else
				{
				     sprintf(temp_buf,"%x",addr_buf[i]);
				     strcat(bt_macaddr_buf, temp_buf);
				}
				
				//__inf("=%x=",addr_buf[i]);
			 
			}
			lowercase_2_uppercase(&bt_macaddr_buf);   //��Сд��ĸ����ͳһת���ɴ�д
			
           __msg("=============mac addr :%s=========\n",bt_macaddr_buf);
#endif			
		}
		break;
		default:
		break;
	}
	return 0;
}

__s32 sys_bt_set_music_info(void)
{

    switch(system_do_msg[1])
    {
        case 'L'://���
        {
            __u8  *ptr=0;
            __s32 datalen = 0;////���ݳ���
            //__msg("id3 Title %d",system_do_msg[2]);
            memset(bt_ID3_info->title_name, 0, 256);
            datalen = system_do_msg[2];
            ptr     = &(system_do_msg[3]);
            strncpy(bt_ID3_info->title_name, ptr, datalen+1);
            __msg("title_name =%s",bt_ID3_info->title_name);

            
        }
        break;
        case 'S'://����Artist
        {
            __u8  *ptr=0;
            __s32 datalen = 0;////���ݳ���
            //__msg("id3 Artist %d",system_do_msg[2]);
            memset(bt_ID3_info->artist_name, 0, 256);
            datalen = system_do_msg[2];
            ptr     = &(system_do_msg[3]);
            strncpy(bt_ID3_info->artist_name, ptr, datalen+1);
            __msg("artist_name =%s",bt_ID3_info->artist_name);
        }
        break;
        case 'A'://����Album
        {
            __u8  *ptr=0;
            __s32 datalen = 0;////���ݳ���
           // __msg("id3 Album %d",system_do_msg[2]);
            memset(bt_ID3_info->album_name, 0, 256);
            datalen = system_do_msg[2];
            ptr     = &(system_do_msg[3]);
            strncpy(bt_ID3_info->album_name, ptr, datalen+1);
            __msg("album_name =%s",bt_ID3_info->album_name);
        }
        break;
        case 'C'://��ǰ����ʱ��
        {
           __u16  cur_time = 0;
           __u8   tmp_time[16];
           __u16  min,sec;
           //__msg("id3 cur time [byte=%d][%d%d]",system_do_msg[2],system_do_msg[3],system_do_msg[4]);
           memset(bt_ID3_info->cur_time, 0, 32);
           cur_time = (system_do_msg[3] << 8) | system_do_msg[4];
           //__msg("cur_time =%d",cur_time);
           min = cur_time/60;
           sec = cur_time%60;
            __msg("cur_min = %d,cur_sec= %d",min,sec);
            if(min > 9)
        	{
        		sprintf(bt_ID3_info->cur_time,"%d",min);
        	}
        	else
        	{
        		sprintf(bt_ID3_info->cur_time,"%d%d",0,min);
        	}
            
        	strcat(bt_ID3_info->cur_time,":");
        	if(sec > 9)
        	{
        		sprintf(tmp_time,"%d",sec);
        	}
        	else
        	{
        		sprintf(tmp_time,"%d%d",0,sec);
        	}
           strcat(bt_ID3_info->cur_time,tmp_time);

           // sprintf(bt_ID3_info->cur_time,"%d/%d",min,sec);
           
        }
        break;
        case 'T'://�ܲ���ʱ��
        {
           __u16  total_time = 0;
           __u8   tmp_time[16];
           __u16  min,sec;
          // __msg("id3 total time [byte=%d][%d%d]",system_do_msg[2],system_do_msg[3],system_do_msg[4]);
           memset(bt_ID3_info->total_time, 0, 32);
           total_time = (system_do_msg[3] << 8) | system_do_msg[4];
          // __msg("total_time =%d",total_time);
           min = total_time/60;
           sec = total_time%60;
           __msg("total_min =%d,total_sec =%d",min,sec);
            if(min > 9)
        	{
        		sprintf(bt_ID3_info->total_time,"%d",min);
        	}
        	else
        	{
        		sprintf(bt_ID3_info->total_time,"%d%d",0,min);
        	}
            
        	 strcat(bt_ID3_info->total_time,":");
        	if(sec > 9)
        	{
        		sprintf(tmp_time,"%d",sec);
        	}
        	else
        	{
        		sprintf(tmp_time,"%d%d",0,sec);
        	}
           strcat(bt_ID3_info->total_time,tmp_time);
           
           //sprintf(bt_ID3_info->total_time,"%d/%d",min,sec);
           
        }
        break;
        default:
        __msg("id3 info err!");
		break;
        
    }
    return 0;
}
//�������ص����ֲ���״̬
 void sys_bt_set_music_status(u8 music_status)
{
	
	__msg("=====bt music status:%x====\n",music_status);
	if(music_status=='A')///stop
	{
		__msg("=================bt music stop==============\n");
		bt_audio_set_status(BT_STAT_PAUSE);
	}
	else if(music_status=='B')///play
	{
		__msg("=================bt music play==============\n");
		bt_audio_set_status(BT_STAT_PLAY);
	}
		
	return;
}

__s32 sys_bt_get_phone_state(void)
{
	if(bt_phone_state ==BT_PHONE_OUTING||bt_phone_state ==BT_PHONE_INING||
		bt_phone_state ==BT_PHONE_TALKINING||bt_phone_state ==BT_PHONE_TALKOUTING)
	{
		__wrn("**********bt calling:%d************\n",bt_phone_state);
		return 1;///�������ڽ���
	}
	else
	{
		//__msg("===========bt free==========\n");
		return 0;
	}
}
__s32 sys_bt_set_phone_state(bt_phone_state_t phone_state)
{
	bt_phone_state = phone_state;
	return 0;
}

__s32 sys_bt_get_phone_type(void)
{
	return bt_phone_type;
}
__s32 sys_bt_set_phone_type(bt_phone_type_t phone_type)
{
	bt_phone_type = phone_type;
	return 0;
}

__s32  sys_bt_is_connected(void)
{
		return bt_is_connected();
}

__s32 sys_bt_spp_rfcomm_is_connected(void)
{
    return bt_spp_rfcomm_is_connected();
}


//������Ҫ�û���ʵ�ִ���SPP���յ������ݣ�͸��
__s32 bt_spp_rx_data(char *CmdStr,unsigned char len) 
{
    u8 oncecmd,seccmd;
    oncecmd = CmdStr[0];
    seccmd = CmdStr[1];
#if RTOS
    phonelink_spp_rx(CmdStr,len);
#endif
    switch(oncecmd)
    {
        case 'A'://auto UUID CmdStr[2]
        {
            if(seccmd == 'A')
            {
               bt_spp_rx_data_rfcomm(&CmdStr[2],len-2);   
               sys_bt_set_phone_type(PHONE_ANDROID_AUTO);
            }
        }
        break;
        case 'B'://wireless bt version CmdStr[2]
        {
            if(seccmd == 'V')
            {
                    
            }
        }
        break;
        case 'D'://�������ɲ�Ҫ
        {
            if(seccmd == 'B')//bt mac addr CmdStr[2]
            {
                   
            }
        }
        break;
        case 'I'://�������ɲ�Ҫ
        {
            if(seccmd == 'A') //disconnect bt return state
            {

            }
            else if(seccmd == 'B')////reconnect bt return state
            {
                   
            }
        }
        break;
        case 'J':
        {
            u8 i ;
            if(seccmd == 'H')//phone mac addr CmdStr[2]
            {
                for(i=0;i<12;i++)
                {
                   __log("%x",CmdStr[2+i]);
                }
                __log("\n");
                
            }
        }
        break;
        case 'N'://�������ɲ�Ҫ
        {
            if(seccmd == 'A')//bt name CmdStr[2],���� hostname
            {
                   
            }
        }
        break;
        case 'P':
        {
            if(seccmd == 0)//��Կ�ʼ
            {
            
            }
            else if(seccmd == 1)//��Գɹ�
            {
            
            }
            else if(seccmd == 2)//���ʧ��
            {
            
            }
           // else if(seccmd == 3)//��Խ���
           // {
            
           // }
        }
        break;
        case 'S':
        {
            if(seccmd == 'A')//phone name CmdStr[2]
            {
            
            }
            else  if(seccmd == 'I')//rfcomm data CmdStr[2]
            {
                bt_spp_rx_data_rfcomm(&CmdStr[2],len-2);
            }
            else  if(seccmd == 'V')//rfcomm connected return state
            {
                
            }
            else  if(seccmd == 'S')//rfcomm disconnected return state
            {
                
            }
        }
        break;
        case 'U':
        {
            if(seccmd == 'S')//UUID start
            {
            
            }
            else  if(seccmd == 'U')//carplay UUID data CmdStr[2]
            {
                bt_spp_rx_data_rfcomm(&CmdStr[2],len-2);
                sys_bt_set_phone_type(PHONE_CARPLAY);
            }
            else  if(seccmd == 'E')//UUID end
            {
                
            }    
        }
        break;
        default:
        break;
    }
    return EPDK_OK;
}

//������Ҫ�û���ʵ�ִ���BLE���յ������ݣ�͸��
__s32 bt_ble_rx_data(char *CmdStr,unsigned char len) 
{
#if RTOS
   // phonelink_ble_rx(CmdStr,len);
#endif
    sys_bt_set_phone_type(PHONE_HARMONY);
    switch(CmdStr[0])
    {






    }
    return EPDK_OK;
}



/**************************bt uart******************************
���º�����ϵͳ��ο���������Ҫ�û���ʵ��,
uartͨѶĬ�ϲ�����115200��8λ����λ��1λֹͣλ����У��λ
**************************bt uart******************************/

__s32 	uart_init = 0;////1-init,0-uninit
__s32 com_uart_init(void)
{
	if(0 == uart_init)
	{
	    uart_init = 1;
#if RTOS 
      rtos_com_uart_init();
#endif
#if RTT 
      rtt_com_uart_init();
#endif
#if LINUX 
       linux_com_uart_init();
#endif
    }
	return EPDK_OK;
}

__s32 com_uart_deinit(void)
{
    if(1 == uart_init)
	{
#if RTOS 
        rtos_com_uart_deinit();
#endif
#if RTT 
        rtt_com_uart_deinit();
#endif
#if LINUX 
        linux_com_uart_deinit();
#endif
        uart_init = 0;
    }
	return EPDK_OK;
}

__s32 com_uart_write(char* pbuf, __s32 size)
{
#if RTOS 
    rtos_com_uart_write(pbuf, size);
#endif
#if RTT 
    rtt_com_uart_write(pbuf, size);
#endif
#if LINUX 
    linux_com_uart_write(pbuf, size);
#endif
	return EPDK_OK;
}

__s32 com_uart_read(char* pbuf, __s32 buf_size, __s32* size)
{
#if RTOS 
    rtos_com_uart_read(pbuf,buf_size,size);
#endif
#if RTT 
    rtt_com_uart_read(pbuf,buf_size,size);
#endif
#if LINUX 
    linux_com_uart_read(pbuf,buf_size,size);
#endif
	return EPDK_OK;
}

__s32 com_uart_flush(void)
{
#if RTOS 
    rtos_com_uart_flush();
#endif
#if RTT 
    rtt_com_uart_flush();
#endif
#if LINUX 
    linux_com_uart_flush();
#endif
	return EPDK_OK;
}
////1-init,0-uninit
__s32 com_uart_state(void)
{
	return uart_init;
}

__s32 bt_para_init(void)
{
#if RTOS  
	bt_phonebook_para = (bt_phonebook_state *)esMEMS_Balloc(sizeof(bt_phonebook_state));
	if(bt_phonebook_para == NULL)
	{
		printf("=============Request phone book fail=============\n");	
		goto para_exit;
	}	

	bt_ID3_info = (bt_ID3_info_t *)esMEMS_Balloc(sizeof(bt_ID3_info_t));
	if(bt_ID3_info == NULL)
	{
		printf("=============Request bt_ID3_info fail=============\n");	
		goto para_exit;
	}
	
    memset((void *)bt_phonebook_para, 0, sizeof(bt_phonebook_state));
    memset((void *)bt_ID3_info, 0, sizeof(bt_ID3_info_t));
#endif    	

    return EPDK_OK;
para_exit:

#if RTOS  
    printf("=============Request memory fail=============\n");	
    if(bt_ID3_info)
    {
         memset(bt_ID3_info, 0, sizeof(bt_ID3_info_t));
         esMEMS_Bfree(bt_ID3_info,sizeof(bt_ID3_info_t));
    }
    if(bt_phonebook_para)
    {
         memset(bt_phonebook_para, 0, sizeof(bt_phonebook_state));
         esMEMS_Bfree(bt_phonebook_para,sizeof(bt_phonebook_state));
    }  
#endif    
    return EPDK_FAIL;
    
}

__s32 bt_para_exit(void)
{

#if RTOS    
    if(bt_ID3_info)
    {
         memset(bt_ID3_info, 0, sizeof(bt_ID3_info_t));
        esMEMS_Bfree(bt_ID3_info,sizeof(bt_ID3_info_t));
    }
    if(bt_phonebook_para)
    {
         memset(bt_phonebook_para, 0, sizeof(bt_phonebook_state));
        esMEMS_Bfree(bt_phonebook_para,sizeof(bt_phonebook_state));
    }    
#endif    
    return EPDK_OK;
}


__s32 bt_task_start(void)
{
#if RTOS 
    rtos_bt_task_start();
#endif
#if RTT 
    rtt_bt_task_start();
#endif
#if LINUX 
    linux_bt_task_start();
#endif	
	return EPDK_OK;

}

__s32 bt_task_stop(void)
{
#if RTOS 
    rtos_bt_task_stop();
#endif
#if RTT 
    rtt_bt_task_stop();
#endif
#if LINUX 
    linux_bt_task_stop();
#endif
	return EPDK_OK;
}


void bt_msleep(uint16_t ticks)
{

#if RTOS 
    rtos_msleep(ticks);
#endif
#if RTT 
    rtt_msleep(ticks);
#endif
#if LINUX 
    linux_msleep(ticks);
#endif
    return;
}


#ifdef __cplusplus
}
#endif
