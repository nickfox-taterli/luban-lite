

#include "bt_os.h"

#ifdef __cplusplus
extern "C" {
#endif



#define 		BT_TEXT_LEN					718

static uint8_t bt_play_status = 0;

static u8 tmp_spp_txdata[528]={'\0'};//spp rfcomm ͨѶ���͸����������ݣ��������������������͸����ȥ�����ݵ�
static u8 spp_txdata[528]={'\0'};    //spp rfcomm ͨѶ���͸����������ݣ��������������������͸����ȥ�����ݵ�
 u8 spp_rxdata[512]={'\0'}; //rfcomm ͨѶ����ֻ�����͸��������


//��Сд��ĸת���ɴ�С��ĸ
__s32  lowercase_2_uppercase(char *str)
{
    char *p_str = NULL ;

    if (!str)
    {
        __msg("string is null , return \n");
        return EPDK_FAIL ;
    }

    p_str = str ;

    while (*p_str != '\0')
    {
        if ((*p_str <= 'z') && (*p_str >= 'a'))
        {
            *p_str =  *p_str - 'a' + 'A' ;
        }

        p_str++ ;
    }

    return EPDK_OK ;
}

void BtCmdSend (char *CmdStr,unsigned char len)
{
	char senddata[128]={'\0'};
	char *p = NULL;
	__s32 i = 0;

	p = CmdStr;

#if(comm_mode == 1)
    senddata[0] = 'A';
	senddata[1] = 'T';
    senddata[2] = '#';
    memcpy(&senddata[3],CmdStr,len);

	senddata[len+3]='\r';
	senddata[len+4]='\n';
    __wrn("======send cmd:%s,len:%d======\n",senddata,strlen(senddata));
	com_uart_write(senddata, (5+len));
#elif(comm_mode == 2)
    senddata[0] = 'A';
	senddata[1] = 'T';
    senddata[2] = '#';
    memcpy(&senddata[3],CmdStr,len);

	senddata[len+3]='\\';
	senddata[len+4]='r';
	senddata[len+5]='\\';
	senddata[len+6]='n';
	__msg("======send cmd:%s,len:%d======\n",senddata,eLIBs_strlen(senddata));
	com_uart_write(senddata, (7+len));
#elif(comm_mode == 3)//USE_0XFF
	senddata[0] = 'A';
	senddata[1] = 'T';
	senddata[2] = len;///'#';
	memcpy(&senddata[3],CmdStr,len);

    senddata[len+3] = 0xFF;
   com_uart_write(senddata, (4+len));
   __msg("======send cmd:%s,len:%d=======\n",senddata,(4+len));
#endif

	return;
}
///////һ���Է����绰���롣
__s32 bt_phone_call(char *phonenumber,__s32 len)
{
	/////AT#CW 18664959468 \r\n
	char call_buf[BT_TEXT_LEN]={"CWN"};///������������
	__s32 i=0;


	__msg("===call len:%x===\n",len);
	eLIBs_memset(call_buf, 0, BT_TEXT_LEN);
	call_buf[0] = 'C';
	call_buf[1] = 'W';
	call_buf[2] = len;
	for(i=0;i<len;i++)
	{
		call_buf[3+i]=phonenumber[i];
	}
	__inf("-call-\n");
	for(i=0;call_buf[i]!=0;i++)
	{
		__inf("-%x-",call_buf[i]);
	}
	__inf("\n");
	__msg("======phone call cmd len:%d=====\n",len+3);
	BtCmdSend (call_buf,(len+3));
	////////////////////////////////////callphone 2016-01-15//////////////////////////////////
	//bt_state =BT_PHONE_OUTING;
	////////////////////////////////////////////////////////////////////////////////////////
	return EPDK_OK;
}
///////ÿ��ֻ�ܰ�һλ�ֻ��š�
__s32 bt_callphone_subnum(char *num)
{
	/////AT#CW 18664959468 -601 \r\n
	/////AT#CX6\r\n
	/////AT#CX0\r\n
	/////AT#CX1\r\n
	char str_buf[BT_TEXT_LEN]={"CX"};

	str_buf[2] = *num;
	__msg("======phone number:%x=======\n",num);
	BtCmdSend (str_buf,3);
	return EPDK_OK;
}
///////�������
__s32 bt_makepair(void)
{
	/////AT#ca\r\n
	char pair[2]={'C','A'};
	__msg("======make pair=======\n");
	BtCmdSend (pair,2);
	return EPDK_OK;
}
///////ȡ�����
__s32 bt_makeunpair(void)
{
	/////AT#cb\r\n
	char pair[2]={'C','B'};
	//__msg("======make unpair=======\n");
	BtCmdSend (pair,2);
	return EPDK_OK;
}
///////�������
__s32 bt_phone_incoming(void)
{
	/////AT#ce\r\n
	char pair[2]={'C','E'};
	//__msg("======incoming=======\n");
	BtCmdSend (pair,2);
	return EPDK_OK;
}
///////�ܽ�����
__s32 bt_phone_reject(void)
{
	/////AT#cf\r\n
	char pair[2]={'C','F'};
	//__msg("======incoming=======\n");
	BtCmdSend (pair,2);
	return EPDK_OK;
}
///////�Ҷϵ绰
__s32 bt_phone_break(void)
{
	/////AT#cg\r\n
	char pair[2]={'C','G'};
	//__msg("======phone break=======\n");
	BtCmdSend (pair,2);
	return EPDK_OK;
}
///////�ز��绰
__s32 bt_phone_recall(void)
{
	/////AT#ch\r\n
	char pair[2]={'C','H'};
	//__msg("======recall=======\n");
	BtCmdSend (pair,2);
	return EPDK_OK;
}
///////vol+
__s32 bt_vol_add(void)
{
	/////AT#ck\r\n
	char pair[2]={'C','K'};
	//__msg("======vol+=======\n");
	BtCmdSend (pair,2);
	return EPDK_OK;
}
///////vol-
__s32 bt_vol_dec(void)
{
	/////AT#cl\r\n
	char pair[2]={'C','L'};
	//__msg("======vol-=======\n");
	BtCmdSend (pair,2);
	return EPDK_OK;
}
///////mute
__s32 bt_vol_mute(void)
{
	///////////////////////////////��������оƬ������//////////////
	/////AT#cm\r\n
	char pair[2]={'A','C'};
	//__msg("======bt_vol_mute=======\n");
	BtCmdSend (pair,2);
	return EPDK_OK;
}
__s32 bt_vol_unmute(void)
{	///////////////////////////////������оƬ������//////////////
	/////AT#cm\r\n
	char pair[2]={'A','O'};
	//__msg("======bt_vol_unmute=======\n");
	BtCmdSend (pair,2);
	return EPDK_OK;
}

///֪ͨbt �ػ���
__s32 bt_power_off(void)
{
	char pair[2]={'S','D'};//shut down
	__msg("======bt_shutdown=======\n");
	BtCmdSend (pair,2);
	return EPDK_OK;
}
__s32 bt_time_get(void)
{
	char pair[2]={'G','T'};
	__msg("======bt_time_get======\n");
	BtCmdSend (pair,2);
	return EPDK_OK;
}
__s32 bt_time_set(void)
{
	/////AT#co\r\n
	char pair[12]={'S','T'};
	__msg("=====bt_time_set====\n");
	pair[0]='S';
	pair[1]='T';
	pair[2]=bt_time.year;
	pair[3]=bt_time.month;
	pair[4]=bt_time.day;
	pair[5]=bt_time.hour;
	pair[6]=bt_time.minute;
	pair[7]=0;
	BtCmdSend (pair,8);
	return EPDK_OK;
}
__s32 bt_ill_get(void)
{
	/////AT#GI\r\n
	char pair[2]={'G','I'};
	__msg("======GET ill=======\n");
	BtCmdSend (pair,2);
	return EPDK_OK;
}

__s32 bt_state_get(void)
{
	/////AT#GS\r\n
	char pair[2]={'G','S'};
	__msg("======GET STATE=======\n");
	BtCmdSend (pair,2);
	return EPDK_OK;
}

__s32 bt_phonename_get(void)
{
	/////AT#GN\r\n
	char pair[2]={'G','N'};
	__msg("======GET PHONENAME=======\n");
	BtCmdSend (pair,2);
	return EPDK_OK;
}

__s32 bt_hostname_get(void)
{
	/////AT#co\r\n
	char pair[2]={'G','B'};
	__msg("======GET HOST NAME=======\n");
	BtCmdSend (pair,2);
	return EPDK_OK;
}

__s32 bt_pincard_get(void)
{
	/////AT#GP\r\n
	char pair[2]={'G','P'};
	__msg("======GET pin card=======\n");
	BtCmdSend (pair,2);
	return EPDK_OK;
}

__s32 bt_macaddr_get(void)
{
	/////AT#co\r\n
	char pair[2]={'G','H'};
	__msg("======GET MAC addr=======\n");
	BtCmdSend (pair,2);
	return EPDK_OK;
}
__s32  bt_is_connected(void)
{
	if(bt_connect_flag	 == BT_STATE_CONNECTED)
		return 1;
	else
		return 0;
}
__s32 bt_reconnect_set(void)
{
	/////AT#co\r\n
	char pair[2]={'G','R'};
	__msg("======set bt reconnect=======\n");
	BtCmdSend (pair,2);
	return EPDK_OK;
}

__s32 bt_disconnect_set(void)
{
	/////AT#co\r\n
	char pair[2]={'G','D'};
	__msg("======set bt disconnect=======\n");
	BtCmdSend (pair,2);
	return EPDK_OK;
}

///////HFP״̬��ѯ
////////����:MG<����> or  ERROR
/////////////////<����> :0-> ready
/////////////////<����> :1-> connected
/////////////////<����> :2-> outgoing call
/////////////////<����> :3-> incoming call
/////////////////<����> :4-> active call
__s32 bt_hfpstate_get(void)
{
	/////AT#cy\r\n
	char pair[3]={'C','Y'};
	__msg("======bt_get_hfpstate=======\n");
	BtCmdSend (pair,2);
	return EPDK_OK;
}

///////audio play/pause
__s32 bt_audio_play_pause(void)
{
	/////AT#MA\r\n
	char pair[2]={'M','A'};
	__msg("======bt_audio_play_pause=======\n");
	BtCmdSend (pair,2);
#if 0
    if(BT_STAT_PLAY == bt_play_status)
    {
        bt_play_status = BT_STAT_PAUSE;
        __wrn("BT_STAT_PLAY [%d]",bt_play_status);
    }
    else if(BT_STAT_PAUSE == bt_play_status)
    {
        bt_play_status = BT_STAT_PLAY;
        __wrn("BT_STAT_PLAY [%d]",bt_play_status);
    }
    else
    {
        bt_play_status = BT_STAT_PLAY;
        __wrn("bt_play_status first?? [%d]",bt_play_status);
    }
 #endif
	return EPDK_OK;
}
///////audia stop
__s32 bt_audio_play(void)
{
	/////AT#MC\r\n
	char pair[2]={'M','Y'};
	__msg("======bt_audio_play=======\n");
	BtCmdSend (pair,2);

	return EPDK_OK;
}

///////audia stop
__s32 bt_audio_pause(void)
{
	/////AT#MC\r\n
	char pair[2]={'M','S'};
	__msg("======bt_audio_pause=======\n");
	BtCmdSend (pair,2);

	return EPDK_OK;
}
///////audia stop
__s32 bt_audio_stop(void)
{
	/////AT#MC\r\n
	char pair[2]={'M','C'};
	__msg("======bt_audio_stop=======\n");
	BtCmdSend (pair,2);

	return EPDK_OK;
}
__s32 bt_audio_prev(void)
{
	/////AT#ME\r\n
	char pair[2]={'M','E'};
	__msg("======channel change=======\n");
	BtCmdSend (pair,2);

	return EPDK_OK;
}
///////audia next
__s32 bt_audio_next(void)
{
	/////AT#MD\r\n
	char pair[2]={'M','D'};
	__msg("======bt_audio_next=======\n");
	BtCmdSend (pair,2);

	return EPDK_OK;
}
///////audio get music status/
__s32 bt_audio_get_status(void)
{
    return bt_play_status;
}
///////audio get music status/
__s32 bt_audio_set_status(uint8_t play_status)
{
     bt_play_status = play_status;
     return EPDK_OK;
}
///////audia next
__s32 bt_phonebook_get(void)
{
	/////AT#MD\r\n
	char pair[2]={'B','A'};
	__msg("======bt_phonebook_get=======\n");
	BtCmdSend (pair,2);
	return EPDK_OK;
}
///////audia channel change
__s32 bt_audio_change(void)
{
	/////AT#CO  or  CP\r\n
	char pair[2]={'C','O'};
	__msg("======bt_audio_change======\n");
	BtCmdSend (pair,2);
	return EPDK_OK;
}
///////audio change///ǿ���е��ֻ���
__s32 bt_audio_to_phone(void)
{
	/////AT#CO  or  CP\r\n
	char pair[2]={'C','P'};
	__msg("======bt_audio_to_phone======\n");
	BtCmdSend (pair,2);
	return EPDK_OK;
}
///////audio change///ǿ���е�������
__s32 bt_audio_to_machine(void)
{
	/////AT#CO  or  CP\r\n
	char pair[2]={'C','M'};
	__msg("======bt_audio_to_machine======\n");
	BtCmdSend (pair,2);
	return EPDK_OK;
}

__s32 bt_eq_user_mode(__u8 index, __u8 gain)
{
	char buf[4]={'\0'};
	buf[0]= 'E';
	buf[1]= 'U';
	buf[2]= index;
	buf[3]= gain;
	__msg("======bt_eq_user_mode======\n");
	BtCmdSend (buf,4);
	return EPDK_OK;
}

__s32 bt_eq_pop_mode(__u8 index, __u8 gain)
{
	char buf[4]={'\0'};
	buf[0]= 'E';
	buf[1]= 'P';
	buf[2]= index;
	buf[3]= gain;
	__msg("======bt_eq_pop_mode======\n");
	BtCmdSend (buf,4);
	return EPDK_OK;
}

__s32 bt_eq_rock_mode(__u8 index, __u8 gain)
{
	char buf[4]={'\0'};
	buf[0]= 'E';
	buf[1]= 'R';
	buf[2]= index;
	buf[3]= gain;
	__msg("======bt_eq_rock_mode======\n");
	BtCmdSend (buf,4);
	return EPDK_OK;
}

__s32 bt_eq_jazz_mode(__u8 index, __u8 gain)
{
	char buf[4]={'\0'};
	buf[0]= 'E';
	buf[1]= 'J';
	buf[2]= index;
	buf[3]= gain;
	__msg("======bt_eq_jazz_mode======\n");
	BtCmdSend (buf,4);
	return EPDK_OK;
}

__s32 bt_eq_classic_mode(__u8 index, __u8 gain)
{
	char buf[4]={'\0'};
	buf[0]= 'E';
	buf[1]= 'C';
	buf[2]= index;
	buf[3]= gain;
	__msg("======bt_eq_classic_mode======\n");
	BtCmdSend (buf,4);
	return EPDK_OK;
}

__s32 bt_eq_gentle_mode(__u8 index, __u8 gain)
{
	char buf[4]={'\0'};
	buf[0]= 'E';
	buf[1]= 'G';
	buf[2]= index;
	buf[3]= gain;
	__msg("======bt_eq_gentle_mode======\n");
	BtCmdSend (buf,4);
	return EPDK_OK;
}

__s32 bt_dac_vol_front(__u8 vol)
{
	char buf[3]={'\0'};
	buf[0]= 'D';
	buf[1]= 'F';
	buf[2]= vol;
	__msg("======bt_dac_vol_front======\n");
	BtCmdSend (buf,3);
	return EPDK_OK;
}

__s32 bt_dac_vol_behind(__u8 vol)
{
	char buf[3]={'\0'};
	buf[0]= 'D';
	buf[1]= 'B';
	buf[2]= vol;
	__msg("======bt_dac_vol_behind======\n");
	BtCmdSend (buf,3);
	return EPDK_OK;
}

__s32 bt_dac_vol_left(__u8 vol)
{
	char buf[3]={'\0'};
	buf[0]= 'D';
	buf[1]= 'L';
	buf[2]= vol;
	__msg("======bt_dac_vol_left======\n");
	BtCmdSend (buf,3);
	return EPDK_OK;
}

__s32 bt_dac_vol_right(__u8 vol)
{
	char buf[3]={'\0'};
	buf[0]= 'D';
	buf[1]= 'R';
	buf[2]= vol;
	__msg("======bt_dac_vol_right======\n");
	BtCmdSend (buf,3);
	return EPDK_OK;
}
/*******************SPP&BLE COMM MODE�������1����2ͨѶ��ʽ**********************/

/****************************spp comm********************************************/

//"AT#"��ͷ��ָ��͸��ͨ��

__s32 bt_spp_tx_data(char *CmdStr,unsigned char len)
{
    char *tmpCmdStr = NULL;
    tmpCmdStr = strstr(CmdStr,"AT#");
    //__wrn("==2==bt_spp_tx cmd:%s ,str_len[%d],len:%d===\n",tmpCmdStr,strlen(tmpCmdStr),tmpCmdStr-CmdStr);
    memset(spp_txdata,0,sizeof(spp_txdata));
    spp_txdata[0] = 'A';
    spp_txdata[1] = 'T';
    spp_txdata[2] = '#';
    spp_txdata[3] = '#';
    memcpy(&spp_txdata[4],tmpCmdStr+3,len-(tmpCmdStr-CmdStr));

#if(comm_mode ==1)//AT##��AT#��1
    spp_txdata[len+4-3] = '\r';
    spp_txdata[len+5-3] = '\n';
    __wrn("==3==bt_spp_tx cmd:%s ,str_len[%d],len:%d====\n",spp_txdata,strlen(spp_txdata),len+3);
    com_uart_write(spp_txdata, strlen(CmdStr)+3);
#elif (comm_mode ==2)
    spp_txdata[len+1] = '\\';
    spp_txdata[len+2] = 'r';
    spp_txdata[len+3] = '\\';
    spp_txdata[len+4] = 'n';
    __wrn("==3==bt_spp_tx cmd:%s ,str_len[%d],len:%d====\n",spp_txdata,strlen(spp_txdata),len+5);
    com_uart_write(spp_txdata, strlen(CmdStr)+5);
#endif

	return EPDK_OK;
}

//���������汾��
__s32 bt_spp_request_bt_version(void)
{
	char buf[5]={'\0'};
	buf[0]= 'A';
	buf[1]= 'T';
	buf[2]= '#';
	buf[3]= 'Q';
	buf[4]= 'V';
	__msg("======bt_spp_request_bt_version======\n");
	bt_spp_tx_data (buf,5);
	return EPDK_OK;
}
//����������Ϣ�����ص����ֻ� name ,mac addr,bt uuid
__s32 bt_spp_request_bt_info(void)
{
	char buf[5]={'\0'};
	buf[0]= 'A';
	buf[1]= 'T';
	buf[2]= '#';
	buf[3]= 'Q';
	buf[4]= 'I';
	__msg("======bt_spp_request_bt_info======\n");
	bt_spp_tx_data (buf,5);
	return EPDK_OK;
}
__s32 bt_spp_rfcomm_is_connected(void)
{
    if(bt_spp_connect_flag == BT_SPP_STATE_CONNECTED)
		return 1;
	else
		return 0;

	return EPDK_OK;
}

__s32 bt_spp_rfcomm_reconnect(void)
{
	char buf[5]={'\0'};
	buf[0]= 'A';
	buf[1]= 'T';
	buf[2]= '#';
	buf[3]= 'S';
	buf[4]= 'P';
	__msg("======bt_spp_connect======\n");
	bt_spp_tx_data (buf,5);
	return EPDK_OK;
}
__s32 bt_spp_rfcomm_disconnect(void)
{
	char buf[5]={'\0'};
	buf[0]= 'A';
	buf[1]= 'T';
	buf[2]= '#';
	buf[3]= 'S';
	buf[4]= 'H';
	__msg("======bt_spp_disconnect======\n");
	bt_spp_tx_data (buf,5);
	return EPDK_OK;
}


//RFCOMM���ӳɹ���AT#SG���ϵ������ֻ������ⷢ�������γ����ݴ���תΪAT##��ͨ��RFCOMM���͸��ֻ�
//libzbt_rfcomm_data_send
__s32 bt_spp_tx_data_rfcomm(char *CmdStr,unsigned char len)
{
    memset(tmp_spp_txdata,0,sizeof(tmp_spp_txdata));
	tmp_spp_txdata[0]= 'A';
	tmp_spp_txdata[1]= 'T';
	tmp_spp_txdata[2]= '#';
	tmp_spp_txdata[3]= 'S';
	tmp_spp_txdata[4]= 'G';
	memcpy(&tmp_spp_txdata[5],CmdStr,len);
    bt_spp_tx_data(tmp_spp_txdata,len+5);
	return EPDK_OK;
}

//RFCOMM���ӳɹ����ֻ��ϲ㷢������RFCOMM���ݴ�������ɷ��͵������ֻ�������
//libzbt_rfcomm_data_recv_CB_init
__s32 bt_spp_rx_data_rfcomm(char *CmdStr,unsigned char len)
{
    memset(spp_rxdata,0,sizeof(spp_rxdata));
	memcpy(&spp_rxdata,CmdStr,len);
	return EPDK_OK;
}

/****************************ble comm************************************/
#if 0

__s32 bt_ble_rfcomm_is_connected(void)
{
    if(bt_ble_connect_flag	 == BT_BLE_STATE_CONNECTED)
		return 1;
	else
		return 0;

	return EPDK_OK;
}
__s32 bt_ble_rfcomm_reconnect(void)
{
	char buf[5]={'\0'};
	buf[0]= 'A';
	buf[1]= 'T';
	buf[2]= '#';
	buf[3]= 'S';
	buf[4]= 'P';
	__msg("======bt_spp_connect======\n");
	bt_spp_tx_data (buf,5);
	return EPDK_OK;
}
__s32 bt_ble_rfcomm_disconnect(void)
{
	char buf[5]={'\0'};
	buf[0]= 'A';
	buf[1]= 'T';
	buf[2]= '#';
	buf[3]= 'S';
	buf[4]= 'H';
	__msg("======bt_spp_disconnect======\n");
	bt_spp_tx_data (buf,5);
	return EPDK_OK;
}
#endif
//RFCOMM���ӳɹ�֮���ϲ㷢��������AT#��ͷ�����ݴ���תΪAT##
__s32 bt_ble_tx_data_rfcomm(char *CmdStr,unsigned char len)
{

    bt_spp_tx_data(CmdStr,len);
	return EPDK_OK;
}

//bt fm api function
/****************************fmrx comm********************************************/

//��̨
__s32 bt_fmrx_search (void)
{
    char buf[5]={'\0'};
	buf[0]= 'A';
	buf[1]= 'T';
	buf[2]= '#';
	buf[3]= 'F';
	buf[4]= 'S';
	__msg("======bt_fmrx_search======\n");
	BtCmdSend (buf,5);
	return EPDK_OK;

}
//��ȡ��ĿƵ������Ϣ���統ǰƵ�㣬�ܽ�Ŀ����
__s32 bt_fmrx_get_info (void)
{
    char buf[5]={'\0'};
	buf[0]= 'A';
	buf[1]= 'T';
	buf[2]= '#';
	buf[3]= 'F';
	buf[4]= 'I';
	__msg("======bt_fmrx_get_info======\n");
	BtCmdSend (buf,5);
	return EPDK_OK;
}
//��ȡ��ĿƵ���б��
__s32 bt_fmrx_get_list (void)
{
    char buf[5]={'\0'};
	buf[0]= 'A';
	buf[1]= 'T';
	buf[2]= '#';
	buf[3]= 'F';
	buf[4]= 'L';
	__msg("======bt_fmrx_get_list======\n");
	BtCmdSend (buf,5);
	return EPDK_OK;
}
//��ȡ�ܽ�Ŀ��
__s32 bt_fmrx_get_ch_cnt (void)
{
    char buf[5]={'\0'};
	buf[0]= 'A';
	buf[1]= 'T';
	buf[2]= '#';
	buf[3]= 'F';
	buf[4]= 'T';
	__msg("======bt_fmrx_get_ch_cnt======\n");
	BtCmdSend (buf,5);
	return EPDK_OK;
}
//���Ž�Ŀ��

__s32 bt_fmrx_set_ch (__u8 ch)
{
    char buf[5]={'\0'};
	buf[0]= 'A';
	buf[1]= 'T';
	buf[2]= '#';
	buf[3]= 'F';
	buf[4]= 'C';
	buf[5]= ch;
	__msg("======bt_fmrx_set_ch======\n");
	BtCmdSend (buf,6);
	return EPDK_OK;
}
//����Ƶ��
__s32 bt_fmrx_set_freq (__u16 freq)
{
    char buf[5]={'\0'};
	buf[0]= 'A';
	buf[1]= 'T';
	buf[2]= '#';
	buf[3]= 'F';
	buf[4]= 'F';
	buf[5]= freq&0xFF;
	buf[6]= (freq>>8)&0xFF;
	__msg("======bt_fmrx_set_freq======\n");
	BtCmdSend (buf,7);
	return EPDK_OK;
}
//�н�Ŀ�ţ�dirΪ1��108M�����У�Ϊ0��78.5M������
__s32 bt_fmrx_switch_channel (__s8 dir)
{
    char buf[5]={'\0'};
	buf[0]= 'A';
	buf[1]= 'T';
	buf[2]= '#';
	buf[3]= 'F';
	buf[4]= 'N';
	buf[5]= dir;
	__msg("======bt_fmrx_switch_channel======\n");
	BtCmdSend (buf,6);
	return EPDK_OK;
}
//��Ƶ�㣬dirΪ1��108M�����У�Ϊ0��78.5M������
__s32 bt_fmrx_switch_freq (__s8 dir)
{
    char buf[5]={'\0'};
	buf[0]= 'A';
	buf[1]= 'T';
	buf[2]= '#';
	buf[3]= 'F';
	buf[4]= 'R';
	buf[5]= dir;
	__msg("======bt_fmrx_switch_freq======\n");
	BtCmdSend (buf,6);
	return EPDK_OK;
}

//��ȡ��Ŀ�Ŷ�Ӧ��Ƶ��
__s32 bt_fmrx_get_ch_to_freq (__u8 ch)
{
    char buf[5]={'\0'};
	buf[0]= 'A';
	buf[1]= 'T';
	buf[2]= '#';
	buf[3]= 'F';
	buf[4]= 'D';
	buf[5]= ch;
	__msg("======bt_fmrx_switch_freq======\n");
	BtCmdSend (buf,6);
	return EPDK_OK;
}
//�л�������Ӧ��,BT����Ĭ������Ӧ��//
//indexֵ����:
//0x41: FMRX;   0x42:AUX2;   0x43:BT;    0x44:MUSIC
__s32 bt_switch_app(u8 index)
{
    char buf[5]={'\0'};
	buf[0]= 'A';
	buf[1]= 'T';
	buf[2]= '#';
	buf[3]= 'C';
	buf[4]= 'O';
	buf[5]= index;
	__msg("======bt_fmrx_switch_freq======\n");
	BtCmdSend (buf,6);


}

void bt_StrToHex(unsigned char *pbDest, unsigned char *pbSrc, int srcLen)
{
    char h1,h2;
    unsigned char s1,s2;
    int i;
    for (i=0; i<srcLen; i++)
    {
        h1 = pbSrc[2*i];
        h2 = pbSrc[2*i+1];
        s1 = eLIBs_toupper(h1) - 0x30; //ʮ������ 0x30, decʮ���� 48	,   ͼ�� 0
        if (s1 > 9)
            s1 -= 7;
        s2 = eLIBs_toupper(h2) - 0x30;
        if (s2 > 9)
            s2 -= 7;
        pbDest[i] = s1*16 + s2;
    }
    return;
}

void bt_HexToStr(char *pszDest, char *pbSrc, int srcLen)
{
    char    ddl, ddh;
    for (int i = 0; i < srcLen; i++)
    {
        ddh = 48 + pbSrc[i] / 16;
        ddl = 48 + pbSrc[i] % 16;
        if (ddh > 57) ddh = ddh + 7;
        if (ddl > 57) ddl = ddl + 7;
        pszDest[i * 2] = ddh;
        pszDest[i * 2 + 1] = ddl;
    }
    pszDest[srcLen * 2] = '\0';
    return;
}

#ifdef __cplusplus
}
#endif

