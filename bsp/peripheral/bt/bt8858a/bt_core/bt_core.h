#ifndef __BT_CORE_H__
#define __BT_CORE_H__

#ifdef __cplusplus
extern "C" {
#endif


//��Сд��ĸת���ɴ�С��ĸ
__s32  lowercase_2_uppercase(char *str);

//cpu-bt cmd api
void BtCmdSend (char *CmdStr,unsigned char len);

//��ȡ��������ʱ��
__s32 bt_time_get(void);

//���ó�������ʱ��
__s32 bt_time_set(void);

//��ȡ���״̬����Ҫ��������GPIO����
__s32 bt_ill_get(void);

//��ȡ��������״̬��ͨ������UART����صģ���һ������������Ҫִ�У�
//����ֻҪ���ϵ������ͻ��Զ���������״̬����CPU�˱�������ͨ��sys_bt_is_connected��ѯ
__s32 bt_state_get(void);

//��ȡ�ֻ����ӵ����֣�����Ĭ�϶��ǳ����б��0�������ֻ��������ӵĳ���������
//�������������������������б�ȥ�����ֻ�
__s32 bt_phonename_get(void);

//��ȡ��������������С��24�ֽ�
__s32 bt_hostname_get(void);

//��ȡ����������PIN��(4λ���������)�����ڲ���
__s32 bt_pincard_get(void);

///HFP״̬��ѯ
__s32 bt_get_hfpstate(void);

//�������ֲ���ֹͣ������һ������һ���෴
__s32 bt_audio_play_pause(void);

//�������ֲ���
__s32 bt_audio_play(void);

//����������ͣ
__s32 bt_audio_pause(void);

//��������ֹͣ
__s32 bt_audio_stop(void);

//����������һ��
__s32 bt_audio_prev(void);

//����������һ��
__s32 bt_audio_next(void);

//��ȡ�������ֵı��ز���״̬,�������ֻ��ϲ����������ִ�����״̬��,Ȼ����ܵ���
__s32 bt_audio_get_status(void);

//��ȡ�������ֵı��ز���״̬,�������ֻ��ϲ����������ִ�����״̬��,Ȼ����ܵ���
__s32 bt_audio_set_status(uint8_t play_status);

//��ȡ�ֻ��绰����ͨ����¼�ȣ���¼��bt_phonebook_state���ݽṹ����
__s32 bt_phonebook_get(void);

//�л���Ƶͨ��
__s32 bt_audio_change(void);

//ǿ���е��ֻ���
__s32 bt_audio_to_phone(void);

//ǿ���е�������
__s32 bt_audio_to_machine(void);

//֪ͨbt power off CPU
//BT����Դ����ʱ���ſ��Կ���CPU��Դ
__s32 bt_power_off(void);

//�������
__s32 bt_phone_incoming(void);

//һ���Է����绰���롣
__s32 bt_phone_call(char *phonenumber,__s32 len);

//�Ҷϵ绰
__s32 bt_phone_break(void);

//�ܽ�����
__s32 bt_phone_reject(void);

//�ز��绰
__s32 bt_phone_recall(void);

//BT�Ƿ������� ?
__s32  bt_is_connected(void);

//��������BT
__s32 bt_reconnect_set(void);

//�Ͽ�����BT
__s32 bt_disconnect_set(void);

//��ȡ��������MAC��ַ
__s32 bt_macaddr_get(void);

//EQ�û�ģʽ
__s32 bt_eq_user_mode(__u8 index, __u8 gain);

//EQ����ģʽ
__s32 bt_eq_pop_mode(__u8 index, __u8 gain);

//EQҡ��ģʽ
__s32 bt_eq_rock_mode(__u8 index, __u8 gain);

//EQ��ʿģʽ
__s32 bt_eq_jazz_mode(__u8 index, __u8 gain);

//EQ����ģʽ
__s32 bt_eq_classic_mode(__u8 index, __u8 gain);

//EQ���ģʽ
__s32 bt_eq_gentle_mode(__u8 index, __u8 gain);

//��Чǰ��
__s32 bt_dac_vol_front(__u8 vol);

//��Ч����
__s32 bt_dac_vol_behind(__u8 vol);

//��Ч����
__s32 bt_dac_vol_left(__u8 vol);

//��Ч����
__s32 bt_dac_vol_right(__u8 vol);

//vol+
__s32 bt_vol_add(void);

//vol-
__s32 bt_vol_dec(void);

//����
__s32 bt_vol_mute(void);

//�������
__s32 bt_vol_unmute(void);

/*********************SPP COMM******************************/
//"AT#"��ͷ��ָ��͸��ͨ��
//����͸������
extern __s32 bt_spp_tx_data (char *CmdStr,unsigned char len);
//����͸������
extern __s32 bt_spp_rx_data(char *CmdStr,unsigned char len);

//�ж�SPP RFCOMM�Ƿ����ӳɹ�
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

//extern __s32 bt_ble_rfcomm_reconnect(void);

//extern __s32 bt_ble_rfcomm_disconnect(void);

extern __s32 bt_ble_tx_data_rfcomm(char *CmdStr,unsigned char len); 

//extern __s32 bt_ble_rx_data_rfcomm(char *CmdStr,unsigned char len);


/*********************FMRX COMM******************************/
//��̨
__s32 bt_fmrx_search (void);

//��ȡ��ĿƵ������Ϣ���統ǰƵ�㣬�ܽ�Ŀ��
__s32 bt_fmrx_get_info (void);

//��ȡ��ĿƵ���б�
__s32 bt_fmrx_get_list (void);

//��ȡ�ܽ�Ŀ��
__s32 bt_fmrx_get_ch_cnt (void);

//���Ž�Ŀ��
__s32 bt_fmrx_set_ch (__u8 ch);

//����Ƶ��
__s32 bt_fmrx_set_freq (__u16 freq);

//�н�Ŀ�ţ�dirΪ1��108M�����У�0��78.5M������
__s32 bt_fmrx_switch_channel (__s8 dir);

//��Ƶ�㣬dirΪ1��108M�����У�0��78.5M������
__s32 bt_fmrx_switch_freq (__s8 dir);

//��ȡ��Ŀ�Ŷ�Ӧ��Ƶ��
__s32 bt_fmrx_get_ch_to_freq (__u8 ch);

//�л�������Ӧ��,BT����Ĭ������Ӧ��//
//indexֵ����:
//0x41: FMRX;   0x42:AUX2;   0x43:BT;    0x44:MUSIC
__s32 bt_switch_app(u8 index);

//�ַ�ת����ʮ��������ʾ��ת���󳤶ȼ��� srcLen/2
void bt_StrToHex(unsigned char *pbDest, unsigned char *pbSrc, int srcLen);
//ʮ������ת�����ַ���ʾ��ת���󳤶ȼӱ� srcLen*2
void bt_HexToStr(char *pszDest, char *pbSrc, int srcLen);


#ifdef __cplusplus
}
#endif

#endif /* __BT_CORE_H__ */
