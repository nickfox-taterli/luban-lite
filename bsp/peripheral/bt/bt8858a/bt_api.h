#ifndef __BT_API_H__
#define __BT_API_H__

#ifdef __cplusplus
extern "C" {
#endif
#include "bt_type.h"
#include "bt_config.h"
#include "aic_log.h"

#define 	        BT_MSG_MAX 	     512//BT MSG datalen
#define 	        BT_CMD_MAX 	     512// ������512��Ԫ��

//�������ֲ���״̬��ȡ
typedef enum __BT_STATUS
{
    BT_STAT_IDLE = 0,
    BT_STAT_PLAY ,
    BT_STAT_PAUSE,
    BT_STAT_
} bt_status_t;

//����ʱ���ȡ
typedef struct
{
   	__u8 year;
	__u8 month;
	__u8 day;
	__u8 hour;
	__u8 minute;
	__u8 second;
}bt_time_t;

/*******bt*******/
//�����绰����ȡ
typedef struct
{
//////////////////�绰��
	__u8 	phonebook_name[1000][64];			//����
	__u8 	phonebook_number[1000][32];			//�绰�������������ϵĵ绰�������ͬһ������ʾ
	__s32 	cur_phone_total;						//���һ��
//////////////////ͨ����¼
	//__u8 	phonerem_name[64][64];			//ͨ����¼,���ڷ���
	//__u8 	phonerem_number[64][32];			//�绰�������������ϵĵ绰�������ͬһ������ʾ
	//__u8 	phonerem_date[64][32];			////����
	//__u8 	phonerem_type[64];			////����////1:���2:�����3 :�ܽ�
	//__s32 	cur_phonerem_total;

	__u8 	phoneout_name[32][64];			//���
	__u8 	phoneout_number[32][32];		//�绰�������������ϵĵ绰�������ͬһ������ʾ
	__u8 	phoneout_date[32][32];			////����
	__s32 	cur_phoneout_total;

	__u8 	phonein_name[32][64];			//���
	__u8 	phonein_number[32][32];		//�绰�������������ϵĵ绰�������ͬһ������ʾ
	__u8 	phonein_date[32][32];			////����
	__s32 	cur_phonein_total;

	__u8 	phonemiss_name[32][64];			//δ��
	__u8 	phonemiss_number[32][32];		//�绰�������������ϵĵ绰�������ͬһ������ʾ
	__u8 	phonemiss_date[32][32];			////����
	__s32 	cur_phonemiss_total;

}bt_phonebook_state;


//��������ID3��Ϣ��ȡ
typedef struct BT_ID3_INFO
{
    __u8  title_name[256];//���
    __u8  artist_name[256];//����
    __u8  album_name[256];//����
    __u8  cur_time[32];    //��ǰ����ʱ��
    __u8  total_time[32];  //������ʱ��

}bt_ID3_info_t;

//������������״̬
typedef enum
{
	BT_STATE_INIT = 0,
	BT_STATE_NOTCONNECTE,
	BT_STATE_CONNECTED,
	BT_STATE_MAX,
}bt_connect_flag_t;

typedef enum
{
	BT_SPP_STATE_INIT = 0,
	BT_SPP_STATE_NOTCONNECTE,
	BT_SPP_STATE_CONNECTED,
	BT_SPP_STATE_MAX,
}bt_spp_connect_flag_t;

typedef enum
{
	BT_BLE_STATE_INIT = 0,
	BT_BLE_STATE_NOTCONNECTE,
	BT_BLE_STATE_CONNECTED,
	BT_BLE_STATE_MAX,
}bt_ble_connect_flag_t;

//�����绰״̬
typedef enum
{
    BT_PHONE_INIT = 0,               	////��ʼ����һ�����
    BT_PHONE_INING,          	////�绰���
    BT_PHONE_OUTING,	  	////�绰���
    BT_PHONE_TALKINING,       ////ͨ����
    BT_PHONE_TALKOUTING,       ////ͨ����
    BT_MAX
}bt_phone_state_t;

//�ֻ���������
typedef enum
{
    PHONE_ANDROID_AUTO,          	////androidϵͳԭ���ֻ�
    PHONE_CARPLAY ,                ////ƻ���ֻ�
    PHONE_ANDROID_CARLIFE,	  	////androidϵͳħ���ֻ�
    PHONE_HARMONY,	  	////HARMONY�ֻ�
    PHONE_MAX
}bt_phone_type_t;


extern u8 	phone_macaddr_buf[12];//�ֻ�����MAC ADDR
extern u8 	phone_name_buf[24];//�ֻ���������
extern u8 	bt_hostname_buf[24];//������������
extern u8 	bt_pincard_buf[4]; //���������
extern u8 	bt_macaddr_buf[12];//��������MAC ADDR
extern u8	bt_callin_buf[64];//�ֻ��������
extern u8   spp_rxdata[512];  //rfcomm ͨѶ����ֻ�����͸��������
extern u8   system_do_msg[BT_MSG_MAX];	//ϵͳҪ�������Ϣ
extern bt_connect_flag_t 	 	bt_connect_flag; //�����Ƿ�����
extern bt_spp_connect_flag_t 	bt_spp_connect_flag;//����spp�Ƿ�����
extern bt_ble_connect_flag_t 	bt_ble_connect_flag; //����BLE�Ƿ�����
extern bt_phone_state_t			bt_phone_state;          //�����绰״̬
extern bt_phone_type_t			bt_phone_type;   //�ֻ���������
extern bt_time_t                bt_time;
extern bt_phonebook_state		*bt_phonebook_para;
extern bt_ID3_info_t			*bt_ID3_info;


/******************************FMRX********************************************/
//����¼��BT��FM�ڲ��ϵ�����±������ݣ����BTҪ�ϵ磬��ô��Ҫ��ȡFMRX��LIST�������ض˼�¼
typedef struct {
    u16  freq;  //��ǰƵ��
    u8   ch_cur;//��ǰ��Ŀ
    u8   ch_cnt;//�ܽ�Ŀ��
} bt_fmrx_info_t;

typedef struct {
    u8   ch_cnt;    //�ܽ�Ŀ��
    u16  freq[64];   //��һ����Ŀ�����0��ʼ
} bt_fmrx_list_t;

extern bt_fmrx_info_t			bt_fmrx_info;//ĳ����Ŀ����Ϣ�����ܽ�Ŀ������ǰ��Ŀ�ţ��Լ���Ӧ��Ƶ��
extern bt_fmrx_list_t			bt_fmrx_list;//��Ŀ��Ƶ���б�




//system api

extern __s32  lowercase_2_uppercase(char *str);

//��������
extern __s32 check_pev_key(void);

//��������ʱ��
extern __s32 sys_bt_set_time(u8 *time);

//��ȡ����ʱ���Ӧ��
extern __s32 sys_bt_get_time_to_app(bt_time_t *time);

//����ʱ�������
extern __s32 sys_bt_set_time_to_bt(bt_time_t *time);

//���ƿ�����
extern void sys_bt_ctrl_lcd(u8 lcd_ctrl);

//���ƿ�����
extern void sys_bt_ctrl_light(u8 light_ctrl);

//���Ƶ���Ӱ��
extern void sys_bt_ctrl_pev(u8 pev_ctrl);

//���÷���bt_phonename_get����֮�����º�����ȡ�绰��
extern __s32 sys_bt_set_phonebook(u8 *phonebook);

//������ػ�ȡ�绰����BUFFER
extern void sys_bt_clear_phonebook(void);

//��ȡ����ָʾ�绰����
extern __s32  sys_bt_get_phonenum(char *btstring);

//��������FMRX��Ϣִ�е�һЩ����
extern __s32 sys_bt_fm_process(void);

//����������Ϣִ�е�һЩ����
extern __s32 sys_bt_dosomething(void);

//��ȡ���ŵ��������ֵ�ID3����Ϣ
extern __s32 sys_bt_set_music_info(void);

//��ȡ���ŵ��������ֵ�״̬(������ͣ)����Ϣ
extern void sys_bt_set_music_status(u8 music_status);

//��ȡ�ֻ��绰BT��״̬
extern __s32 sys_bt_get_phone_state(void);

//bt���������ֻ��绰BT��״̬
extern __s32 sys_bt_set_phone_state(bt_phone_state_t phone_state);

//��ȡ�ֻ�����
__s32 sys_bt_get_phone_type(void);

//bt���������ֻ�����
__s32 sys_bt_set_phone_type(bt_phone_type_t phone_type);

//��ȡ����BT���ӵ�״̬����CPU�˵ı�����ѯ
extern __s32 sys_bt_is_connected(void);

//cpu-bt cmd api
extern void BtCmdSend (char *CmdStr,unsigned char len);

//��ȡ��������ʱ��
extern __s32 bt_time_get(void);

//���ó�������ʱ��
extern __s32 bt_time_set(void);

//��ȡ���״̬����Ҫ��������GPIO����
extern __s32 bt_ill_get(void);

//��ȡ��������״̬��ͨ������UART����صģ���һ������������Ҫִ�У�
//����ֻҪ���ϵ������ͻ��Զ���������״̬����CPU�˱�������ͨ��sys_bt_is_connected��ѯ
extern __s32 bt_state_get(void);

//��ȡ�ֻ����ӵ����֣�����Ĭ�϶��ǳ����б��0�������ֻ��������ӵĳ���������
//�������������������������б�ȥ�����ֻ�
extern __s32 bt_phonename_get(void);

//��ȡ��������������С��24�ֽ�
extern __s32 bt_hostname_get(void);

//��ȡ����������PIN��(4λ���������)�����ڲ���
extern __s32 bt_pincard_get(void);

///HFP״̬��ѯ
extern __s32 bt_get_hfpstate(void);

//�������ֲ���ֹͣ������һ������һ���෴
extern __s32 bt_audio_play_pause(void);

//�������ֲ���
extern __s32 bt_audio_play(void);

//����������ͣ
extern __s32 bt_audio_pause(void);

//��������ֹͣ
extern __s32 bt_audio_stop(void);

//����������һ��
extern __s32 bt_audio_prev(void);

//����������һ��
extern __s32 bt_audio_next(void);

//��ȡ�������ֵı��ز���״̬,�������ֻ��ϲ����������ִ�����״̬��,Ȼ����ܵ���
extern __s32 bt_audio_get_status(void);

//��ȡ�������ֵı��ز���״̬,�������ֻ��ϲ����������ִ�����״̬��,Ȼ����ܵ���
extern __s32 bt_audio_set_status(uint8_t play_status);

//��ȡ�ֻ��绰����ͨ����¼�ȣ���¼��bt_phonebook_state���ݽṹ����
extern __s32 bt_phonebook_get(void);

//�л���Ƶͨ��
extern __s32 bt_audio_change(void);

//ǿ���е��ֻ���
extern __s32 bt_audio_to_phone(void);

//ǿ���е�������
extern __s32 bt_audio_to_machine(void);

//֪ͨbt power off CPU
//BT����Դ����ʱ���ſ��Կ���CPU��Դ
extern __s32 bt_power_off(void);

//�������
extern __s32 bt_phone_incoming(void);

//һ���Է����绰���롣
extern __s32 bt_phone_call(char *phonenumber,__s32 len);

//�Ҷϵ绰
extern __s32 bt_phone_break(void);

//�ܽ�����
extern __s32 bt_phone_reject(void);

//�ز��绰
extern __s32 bt_phone_recall(void);

//BT�Ƿ������� ?
extern __s32  bt_is_connected(void);

//��������BT
extern __s32 bt_reconnect_set(void);

//�Ͽ�����BT
extern __s32 bt_disconnect_set(void);

//��ȡ��������MAC��ַ
extern __s32 bt_macaddr_get(void);

//EQ�û�ģʽ
extern __s32 bt_eq_user_mode(__u8 index, __u8 gain);

//EQ����ģʽ
extern __s32 bt_eq_pop_mode(__u8 index, __u8 gain);

//EQҡ��ģʽ
extern __s32 bt_eq_rock_mode(__u8 index, __u8 gain);

//EQ��ʿģʽ
extern __s32 bt_eq_jazz_mode(__u8 index, __u8 gain);

//EQ����ģʽ
extern __s32 bt_eq_classic_mode(__u8 index, __u8 gain);

//EQ���ģʽ
extern __s32 bt_eq_gentle_mode(__u8 index, __u8 gain);

//��Чǰ��
extern __s32 bt_dac_vol_front(__u8 vol);

//��Ч����
extern __s32 bt_dac_vol_behind(__u8 vol);

//��Ч����
extern __s32 bt_dac_vol_left(__u8 vol);

//��Ч����
extern __s32 bt_dac_vol_right(__u8 vol);

//vol+
extern __s32 bt_vol_add(void);

//vol-
extern __s32 bt_vol_dec(void);

//����
extern __s32 bt_vol_mute(void);

//�������
extern __s32 bt_vol_unmute(void);

/*********************SPP COMM******************************/
//"AT#"��ͷ��ָ��͸��ͨ��
//����͸������
extern __s32 bt_spp_tx_data (char *CmdStr,unsigned char len);
//����͸������
extern __s32 bt_spp_rx_data(char *CmdStr,unsigned char len);

extern __s32 bt_spp_rfcomm_is_connected(void);

extern __s32 bt_spp_rfcomm_reconnect(void);

extern __s32 bt_spp_rfcomm_disconnect(void);

//RFCOMM���ӳɹ���AT#SG���ϵ������ֻ������ⷢ�������γ����ݴ���תΪAT##��ͨ��RFCOMM���͸��ֻ�
extern __s32 bt_spp_tx_data_rfcomm(char *CmdStr,unsigned char len);

//RFCOMM���ӳɹ����ֻ��ϲ㷢������RFCOMM���ݴ�������ɷ��͵������ֻ�������
extern __s32 bt_spp_rx_data_rfcomm(char *CmdStr,unsigned char len);

/*********************BLE COMM******************************/

//extern __s32 bt_ble_tx_data(char *CmdStr,unsigned char len);

//extern __s32 bt_ble_rx_data(char *CmdStr,unsigned char len);

//extern __s32 bt_ble_rfcomm_is_connected(void);

//extern __s32 bt_ble_rfcomm_is_reconnect(void);

//extern __s32 bt_ble_rfcomm_disconnect(void);

extern __s32 bt_ble_tx_data_rfcomm(char *CmdStr,unsigned char len);

//extern __s32 bt_ble_rx_data_rfcomm(char *CmdStr,unsigned char len);

/*********************FMRX COMM******************************/
//��̨
extern __s32 bt_fmrx_search (void);

//��ȡ��ĿƵ������Ϣ���統ǰƵ�㣬�ܽ�Ŀ��
extern __s32 bt_fmrx_get_info (void);

//��ȡ��ĿƵ���б�
extern __s32 bt_fmrx_get_list (void);

//��ȡ�ܽ�Ŀ��
extern __s32 bt_fmrx_get_ch_cnt (void);

//���Ž�Ŀ��
extern __s32 bt_fmrx_set_ch (__u8 ch);

//����Ƶ��
extern __s32 bt_fmrx_set_freq (__u16 freq);

//�н�Ŀ�ţ�dirΪ1��108M�����У�0��78.5M������
extern __s32 bt_fmrx_switch_channel (__s8 dir);

//��Ƶ�㣬dirΪ1��108M�����У�0��78.5M������
extern __s32 bt_fmrx_switch_freq (__s8 dir);

//��ȡ��Ŀ�Ŷ�Ӧ��Ƶ��
__s32 bt_fmrx_get_ch_to_freq (__u8 ch);

/*********************BT MOUDLE******************************/

//�л�������Ӧ��,BT����Ĭ������Ӧ��//
//indexֵ����:
//0x41: FMRX;   0x42:AUX2;   0x43:BT;    0x44:MUSIC
extern __s32 bt_switch_app(u8 index);

//�ַ�ת����ʮ��������ʾ��ת���󳤶ȼ��� srcLen/2
extern void bt_StrToHex(unsigned char *pbDest, unsigned char *pbSrc, int srcLen);
//ʮ������ת�����ַ���ʾ��ת���󳤶ȼӱ� srcLen*2
extern void bt_HexToStr(char *pszDest, char *pbSrc, int srcLen);


//bt_init ��ʼ���ڲ����ݽṹ�ʹ����߳�/��ʱ��
extern __s32 bt_init(void);
//bt_exit ��ʼ���ڲ����ݽṹ���ͷ��߳�/��ʱ��
extern __s32 bt_exit(void);

#ifdef __cplusplus
}
#endif

#endif


