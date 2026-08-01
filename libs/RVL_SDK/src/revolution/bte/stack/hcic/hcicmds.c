// Auto-scaffolded catalog TU for RVL_SDK/src/revolution/bte/stack/hcic/hcicmds
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

extern void *GKI_getpoolbuf(unsigned char pool_id);

void btsnd_hcic_inquiry() {}

int btsnd_hcic_inq_cancel(void)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 3;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 2;
    p[9] = 4;
    p[10] = 0;
    btu_hcif_send_cmd(p);
    return 1;
}

void btsnd_hcic_per_inq_mode() {}

void btsnd_hcic_create_conn() {}

void btsnd_hcic_disconnect() {}

void btsnd_hcic_add_SCO_conn() {}

void btsnd_hcic_accept_conn(unsigned char *p, unsigned char *bd_addr, unsigned char role)
{
    *(unsigned short *)(p + 2) = 0xa;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 9;
    p[9] = 4;
    p[10] = 7;
    p[11] = bd_addr[5];
    p[12] = bd_addr[4];
    p[13] = bd_addr[3];
    p[14] = bd_addr[2];
    p[15] = bd_addr[1];
    p[16] = bd_addr[0];
    p[17] = role;
    btu_hcif_send_cmd(p);
}

void btsnd_hcic_reject_conn(unsigned char* p, unsigned char* bd_addr, unsigned char reason)
{
    *(unsigned short*)(p + 2) = 0xa;
    *(unsigned short*)(p + 4) = 0;
    p[8] = 0xa;
    p[9] = 4;
    p[10] = 7;
    p[11] = bd_addr[5];
    p[12] = bd_addr[4];
    p[13] = bd_addr[3];
    p[14] = bd_addr[2];
    p[15] = bd_addr[1];
    p[16] = bd_addr[0];
    p[17] = reason;
    btu_hcif_send_cmd(p);
}

void btsnd_hcic_link_key_req_reply() {}

void btsnd_hcic_link_key_neg_reply() {}

void btsnd_hcic_pin_code_req_reply() {}

void btsnd_hcic_pin_code_neg_reply() {}

void btsnd_hcic_change_conn_type() {}

int btsnd_hcic_auth_request(unsigned short handle)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 5;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x11;
    p[9] = 0x04;
    p[10] = 2;
    p[11] = (unsigned char)handle;
    p[12] = (unsigned char)(handle >> 8);
    btu_hcif_send_cmd(p);
    return 1;
}

void btsnd_hcic_set_conn_encrypt() {}

void btsnd_hcic_rmt_name_req() {}

void btsnd_hcic_rmt_name_req_cancel() {}

int btsnd_hcic_rmt_features_req(unsigned short handle)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 5;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x1b;
    p[9] = 0x04;
    p[10] = 2;
    p[11] = (unsigned char)handle;
    p[12] = (unsigned char)(handle >> 8);
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_rmt_ver_req(unsigned short handle)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 5;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x1d;
    p[9] = 0x04;
    p[10] = 2;
    p[11] = (unsigned char)handle;
    p[12] = (unsigned char)(handle >> 8);
    btu_hcif_send_cmd(p);
    return 1;
}

void btsnd_hcic_read_rmt_clk_offset() {}

void btsnd_hcic_setup_esco_conn() {}

void btsnd_hcic_accept_esco_conn() {}

void btsnd_hcic_reject_esco_conn(void *p_cmd, unsigned char *bd_addr, unsigned char reason)
{
    unsigned char *p = (unsigned char *)p_cmd;
    *(unsigned short *)(p + 2) = 0xa;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x2a;
    p[9] = 4;
    p[10] = 7;
    p[11] = bd_addr[5];
    p[12] = bd_addr[4];
    p[13] = bd_addr[3];
    p[14] = bd_addr[2];
    p[15] = bd_addr[1];
    p[16] = bd_addr[0];
    p[17] = reason;
    btu_hcif_send_cmd(p_cmd);
}

void btsnd_hcic_hold_mode() {}

void btsnd_hcic_sniff_mode() {}

void btsnd_hcic_exit_sniff_mode() {}

void btsnd_hcic_park_mode() {}

void btsnd_hcic_exit_park_mode() {}

void btsnd_hcic_switch_role() {}

void btsnd_hcic_write_policy_set() {}

int btsnd_hcic_reset(void)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 3;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 3;
    p[9] = 0x0c;
    p[10] = 0;
    btu_hcif_send_cmd(p);
    return 1;
}

void btsnd_hcic_set_event_filter() {}

int btsnd_hcic_write_pin_type(unsigned char pin_type)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 4;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x0a;
    p[9] = 0x0c;
    p[10] = 1;
    p[11] = pin_type;
    btu_hcif_send_cmd(p);
    return 1;
}

void btsnd_hcic_read_stored_key(unsigned char *p, unsigned char *bd_addr, unsigned char read_all)
{
    *(unsigned short *)(p + 2) = 0xa;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0xd;
    p[9] = 0xc;
    p[10] = 7;
    p[11] = bd_addr[5];
    p[12] = bd_addr[4];
    p[13] = bd_addr[3];
    p[14] = bd_addr[2];
    p[15] = bd_addr[1];
    p[16] = bd_addr[0];
    p[17] = read_all;
    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_stored_key() {}

void btsnd_hcic_delete_stored_key() {}

void btsnd_hcic_change_name() {}

void btsnd_hcic_write_page_tout(void *p, unsigned short timeout)
{
    unsigned char *b = (unsigned char *)p;
    *(unsigned short *)(b + 2) = 5;
    *(unsigned short *)(b + 4) = 0;
    b[8] = 0x18;
    b[9] = 0x0c;
    b[10] = 2;
    b[11] = (unsigned char)timeout;
    b[12] = (unsigned char)(timeout >> 8);
    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_scan_enable(void *p, unsigned char mode)
{
    unsigned char *b = (unsigned char *)p;
    *(unsigned short *)(b + 2) = 4;
    *(unsigned short *)(b + 4) = 0;
    b[8] = 0x1a;
    b[9] = 0x0c;
    b[10] = 1;
    b[11] = mode;
    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_pagescan_cfg(void *p, unsigned short interval, unsigned short window)
{
    unsigned char *b = (unsigned char *)p;
    *(unsigned short *)(b + 2) = 7;
    *(unsigned short *)(b + 4) = 0;
    b[8] = 0x1c;
    b[9] = 0x0c;
    b[10] = 4;
    b[11] = (unsigned char)interval;
    b[12] = (unsigned char)(interval >> 8);
    b[13] = (unsigned char)window;
    b[14] = (unsigned char)(window >> 8);
    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_inqscan_cfg(void* p, unsigned short interval, unsigned short window)
{
    unsigned char* b = (unsigned char*)p;
    *(unsigned short*)(b + 2) = 7;
    *(unsigned short*)(b + 4) = 0;
    b[8] = 0x1e;
    b[9] = 0x0c;
    b[10] = 4;
    b[11] = (unsigned char)interval;
    b[12] = (unsigned char)(interval >> 8);
    b[13] = (unsigned char)window;
    b[14] = (unsigned char)(window >> 8);
    btu_hcif_send_cmd(p);
}

int btsnd_hcic_write_auth_enable(unsigned char flag)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 4;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x20;
    p[9] = 0x0c;
    p[10] = 1;
    p[11] = flag;
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_write_encr_mode(unsigned char mode)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 4;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x22;
    p[9] = 0x0c;
    p[10] = 1;
    p[11] = mode;
    btu_hcif_send_cmd(p);
    return 1;
}

void btsnd_hcic_write_dev_class(void *p, unsigned char *dev_class)
{
    unsigned char *b = (unsigned char *)p;
    *(unsigned short *)(b + 2) = 6;
    *(unsigned short *)(b + 4) = 0;
    b[8] = 0x24;
    b[9] = 0x0c;
    b[10] = 3;
    b[11] = dev_class[2];
    b[12] = dev_class[1];
    b[13] = dev_class[0];
    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_auto_flush_tout(void *p, unsigned short handle, unsigned short timeout)
{
    unsigned char *b = (unsigned char *)p;
    *(unsigned short *)(b + 2) = 7;
    *(unsigned short *)(b + 4) = 0;
    b[8] = 0x28;
    b[9] = 0x0c;
    b[10] = 4;
    b[11] = (unsigned char)handle;
    b[12] = (unsigned char)(handle >> 8);
    b[13] = (unsigned char)timeout;
    b[14] = (unsigned char)(timeout >> 8);
    btu_hcif_send_cmd(p);
}

void btsnd_hcic_set_host_buf_size() {}

void btsnd_hcic_write_link_super_tout() {}

void btsnd_hcic_write_cur_iac_lap(unsigned char *p, unsigned char num_laps, unsigned char *lap_array)
{
    unsigned short len = (num_laps << 2) - num_laps + 4;
    unsigned char *pp = p + 12;
    int i;

    *(unsigned short *)(p + 2) = len;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 0x3a;
    p[9] = 0x0c;
    p[10] = (unsigned char)(len - 3);
    p[11] = num_laps;

    for (i = num_laps; i > 0; i--) {
        pp[0] = lap_array[2];
        pp[1] = lap_array[1];
        pp[2] = lap_array[0];
        pp += 3;
        lap_array += 3;
    }
    btu_hcif_send_cmd(p);
}

int btsnd_hcic_read_local_ver(void)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 3;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 1;
    p[9] = 0x10;
    p[10] = 0;
    btu_hcif_send_cmd(p);
    return 1;
}

int btsnd_hcic_read_local_features(void)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 3;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 3;
    p[9] = 0x10;
    p[10] = 0;
    btu_hcif_send_cmd(p);
    return 1;
}

void btsnd_hcic_read_buffer_size(void *p_buf) {
    ((unsigned short *)p_buf)[1] = 3;
    ((unsigned short *)p_buf)[2] = 0;
    ((unsigned char *)p_buf)[8] = 5;
    ((unsigned char *)p_buf)[9] = 0x10;
    ((unsigned char *)p_buf)[10] = 0;
    btu_hcif_send_cmd(p_buf);
}

int btsnd_hcic_read_bd_addr(void)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 3;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 9;
    p[9] = 0x10;
    p[10] = 0;
    btu_hcif_send_cmd(p);
    return 1;
}

void btsnd_hcic_get_link_quality() {}

int btsnd_hcic_read_rssi(unsigned short handle)
{
    unsigned char *p = (unsigned char *)GKI_getpoolbuf(2);
    if (p == NULL)
        return 0;
    *(unsigned short *)(p + 2) = 5;
    *(unsigned short *)(p + 4) = 0;
    p[8] = 5;
    p[9] = 0x14;
    p[10] = 2;
    p[11] = (unsigned char)handle;
    p[12] = (unsigned char)(handle >> 8);
    btu_hcif_send_cmd(p);
    return 1;
}

void btsnd_hcic_set_afh_channels() {}

void btsnd_hcic_write_inqscan_type(void *p, unsigned char type)
{
    unsigned char *b = (unsigned char *)p;
    *(unsigned short *)(b + 2) = 4;
    *(unsigned short *)(b + 4) = 0;
    b[8] = 0x43;
    b[9] = 0x0c;
    b[10] = 1;
    b[11] = type;
    btu_hcif_send_cmd(p);
}

void btsnd_hcic_write_inquiry_mode(void *buf, unsigned char mode)
{
    unsigned short *ps;
    unsigned char *pb;
    ps = (unsigned short *)((unsigned char *)buf + 2);
    *ps = 4;
    ps = (unsigned short *)((unsigned char *)buf + 4);
    *ps = 0;
    pb = (unsigned char *)buf + 8;
    *pb = 0x45;
    pb = (unsigned char *)buf + 9;
    *pb = 0x0c;
    pb = (unsigned char *)buf + 10;
    *pb = 1;
    pb = (unsigned char *)buf + 11;
    *pb = mode;
    btu_hcif_send_cmd(buf);
}

void btsnd_hcic_write_pagescan_type(void *pBuf, unsigned char type) {
    *(unsigned short*)((unsigned char*)pBuf + 2) = 4;
    *(unsigned short*)((unsigned char*)pBuf + 4) = 0;
    *((unsigned char*)pBuf + 8) = 0x47;
    *((unsigned char*)pBuf + 9) = 0x0C;
    *((unsigned char*)pBuf + 10) = 0x01;
    *((unsigned char*)pBuf + 11) = type;
    btu_hcif_send_cmd(pBuf);
}

void btsnd_hcic_vendor_spec_cmd() {}
