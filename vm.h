#pragma once
#include "tim.h"

enum EVT_CODES
{
	EVT_RET = 0xFF00,
	EVT_01,			// 01
	EVT_02,			// 02
	EVT_03,			// 03
	EVT_04,			// 04
	EVT_05,			// 05
	EVT_06,			// 06
	EVT_07,			// 07
	EVT_08,			// 08
	EVT_09,			// 09
	EVT_DIV,		// 0A
	EVT_MUL,		// 0B
	EVT_SUB,		// 0C
	EVT_ADD,		// 0D
	EVT_DEC,		// 0E
	EVT_INC,		// 0F
	EVT_MOV,		// 10
	EVT_EQU,		// 11
	EVT_NEQ,		// 12
	EVT_GRE,		// 13
	EVT_LWR,		// 14
	EVT_GEQ,		// 15
	EVT_LEQ,		// 16
	EVT_17,			// 17
	EVT_18,			// 18
	EVT_19,			// 19
	EVT_1A,			// 1A
	EVT_1B,			// 1B
	EVT_1C,			// 1C
	EVT_1D,			// 1D
	EVT_1E,			// 1E
	EVT_1F,			// 1F
	EVT_ALLEND,		// 20
	EVT_JMP,		// 21
	EVT_CALL,		// 22
	EVT_EVDEF,		// 23
	EVT_STRANGE1,	// 24
	EVT_STRANGE2,	// 25
	EVT_STRANGE3,	// 26
	EVT_STRANGE4,	// 27
	EVT_END,		// 28
	EVT_IF,			// 29
	EVT_WHILE,		// 2A
	EVT_NOP,		// 2B
	EVT_BREAK,		// 2C
	EVT_ENDIF,		// 2D
	EVT_ENDWHILE,	// 2E
	EVT_ELSE,		// 2F
	EVT_MSGINIT,	// 30
	EVT_MSGTYPE,	// 31
	EVT_MSGATTR,	// 32
	EVT_MSGOUT,		// 33
	EVT_SETMARK,	// 34
	EVT_SETWAIT,	// 35
	EVT_MSG_WAIT,	// 36
	EVT_EVSTART,	// 37
	EVT_BGFILEDISP,	// 38
	EVT_BGLOAD,		// 39
	EVT_PALLOAD,	// 3A
	EVT_BGMREQ,		// 3B
	EVT_SPRCLR,		// 3C
	EVT_ABSOBJANIM,	// 3D
	EVT_OBJANIM,	// 3E
	EVT_ALLSPRCLR,	// 3F
	EVT_MSGCLR,		// 40
	EVT_SCREENCLR,	// 41
	EVT_SCREENON,	// 42
	EVT_SCREENOFF,	// 43
	EVT_SCREENIN,	// 44
	EVT_SCREENOUT,	// 45
	EVT_BGDISP,		// 46
	EVT_BGANIM,		// 47
	EVT_BGSCROLL,	// 48
	EVT_PALSET,		// 49
	EVT_BGWAIT,		// 4A
	EVT_WAIT,		// 4B
	EVT_BWAIT,		// 4C
	EVT_BOXFILL,	// 4D
	EVT_BGCLR,		// 4E
	EVT_SETBKCOL,	// 4F
	EVT_MSGCOL,		// 50
	EVT_MSGSPD,		// 51
	EVT_MAPINIT,	// 52
	EVT_MAPLOAD,	// 53
	EVT_MAPDISP,	// 54
	EVT_SPRENT,		// 55
	EVT_SETPROC,	// 56
	EVT_SCEINIT,	// 57
	EVT_USERCTRL,	// 58
	EVT_MAPATTR,	// 59
	EVT_MAPPOS,		// 5A
	EVT_SPRPOS,		// 5B
	EVT_SPRANIM,	// 5C
	EVT_SPRDIR,		// 5D
	EVT_GAMEINIT,	// 5E
	EVT_CONTINIT,	// 5F
	EVT_SCEEND,		// 60
	EVT_MAPSCROLL,	// 61
	EVT_SPRLMT,		// 62
	EVT_SPRWALKX,	// 63
	EVT_ALLSPRDISP,	// 64
	EVT_MAPWRT,		// 65
	EVT_SPRWAIT,	// 66
	EVT_SEREQ,		// 67
	EVT_SNDSTOP,	// 68
	EVT_SESTOP,		// 69
	EVT_BGMSTOP,	// 6A
	EVT_DOORNOSET,	// 6B
	EVT_RAND,		// 6C
	EVT_BTWAIT,		// 6D
	EVT_FAWAIT,		// 6E
	EVT_SCLBLOCK,	// 6F
	EVT_EVSTOP,		// 70
	EVT_SEREQPV,	// 71
	EVT_SEREQSPR,	// 72
	EVT_SCERESET,	// 73
	EVT_BGSPRENT,	// 74
	EVT_BGSPRPOS,	// 75
	EVT_BGSPRSET,	// 76
	EVT_SLANTSET,	// 77
	EVT_SLANTCLR,	// 78
	EVT_DUMMY,		// 79
	EVT_SPCFUNC,	// 7A
	EVT_SEPAN,		// 7B
	EVT_SEVOL,		// 7C
	EVT_BGDISPTRN,	// 7D
	EVT_DEBUG,		// 7E
	EVT_TRACE,		// 7F
	EVT_TMWAIT,		// 80
	EVT_BGSPRANIM,	// 81
	EVT_ABSSPRENT,	// 82
	EVT_NEXTCOM,	// 83
	EVT_WORKCLR,	// 84
	EVT_BGBUFCLR,	// 85
	EVT_ABSBGSPRENT,// 86
	EVT_AVIPLAY,	// 87
	EVT_AVISTOP,	// 88
	EVT_SPRMARK,	// 89
	EVT_BGMATTR,	// 8A
	EVT_BAD_OPC = 0xFFFF
};

struct PT
{
	__int16 x;
	__int16 y;
	__int16 w;
	__int16 h;
	__int16 rw1;
	__int16 rw2;
};

union ADT
{
	DWORD dw;
	struct word { WORD w[2]; } w;
	struct byte { BYTE b[4]; } b;
};

#define SCE_BANKS		6
#define SCE_BANK_SIZE	0x8000

struct VM
{
	VM() { memset(this, 0, sizeof(VM)); }

	void reset_pos();
	void updateIndex();
	void rewindIndex();
	WORD consume();
	WORD consumeInPlace();
	WORD read16();
	short read16s() { return (short)read16(); }
	void read(WORD* dst, int cnt);
	void getstr(char* dst);
	void stop();
	void evt_calc(WORD code);
	void queue(WORD code);
	int  condition(WORD code);

	void op_default(WORD code);
	void op_nop();
	void op_rand();

	void op_jmp();
	void op_call();
	void op_else();
	void op_endif();
	void op_endwhile();
	void op_evdef();
	void op_evret();
	void op_evtstart();
	void op_set_proc();
	void op_set_mark();

	void op_user_ctr();
	void op_spc_func();
	void op_sce_reset();
	void op_sce_init();
	void op_sce_end();
	void op_game_init();
	void op_cont_init();
	void op_slant_set();
	void op_slant_clr();
	void op_scl_block();

	void op_set_bkcol(int size);
	void op_set_msgcol(int size);

	void op_wait();		// timed wait
	void op_fawait();	// fade wait
	void op_btwait();	// button wait
	void op_bg_wait();	// background scroll wait
	void op_tmwait();	// timer wait

	void op_screen_clr();
	void op_box_fill();

	// background sprites
	void op_bg_spr_ent(int is_abs);
	void op_bg_spr_pos();
	void op_bg_spr_set();
	void op_bg_spr_anim();
	// backgrounds
	void op_bg_buf_clr();
	void op_bg_load();
	void op_bg_disp();
	void op_bg_disp_trn();
	void op_bg_file_disp();
	void op_bg_anim();
	void op_bg_scroll();
	void op_bg_clr();

	void op_pal_load();
	void op_pal_set();

	// bgm
	void op_bgm_req();
	void op_bgm_attr();
	// sfx
	void op_se_req();
	void op_se_stop();
	void op_se_vol();
	void op_se_pan();
	void op_se_req_spr();
	void op_se_req_pv();

	// maps
	void op_map_load();
	void op_map_attr();
	void op_map_init(int size);
	void op_map_pos();
	void op_map_disp();
	void op_map_scroll();
	void op_map_wrt();

	// messages
	void op_msg_attr(int size);
	void op_msg_init(int size);
	void op_msg_out();
	void op_msg_wait();
	void op_msg_clr();
	void op_msg_spd(int size);

	// sprites
	void op_spr_clr();
	void op_spr_ent(int is_abs);
	void op_spr_pos();
	void op_spr_anim();
	void op_spr_dir();
	void op_spr_lmt();
	void op_spr_walkx();
	void op_spr_wait();
	void op_spr_mark();

	// objects
	void op_abs_obj_anim();
	void op_obj_anim();

	void op_avi_play();

	int evt_can_stop;
	__int16 field_4[5];
#ifdef _WINDOWS
	HWND hWnd;
#endif
	BYTE* ado[SCE_BANKS];
	ADT adt[2000];
	__int16 r;
	__int16 g;
	__int16 b;
	__int16 _x;
	__int16 _y;
	__int16 _w;
	__int16 _h;
	__int16 _id;
	__int16 field_1F7A;
	PT rects[21];
	WORD msg_basex;
	short msg_basex2;
	WORD msg_w;
	__int16 field_207E;
	__int16 msg_basey;
	WORD msg_v;
	__int16 msg_y;
	WORD msg_x;
	WORD ado_pos0[5];
	WORD ado_pos1[5];
	WORD field_209C[500];
	WORD ado_pos_bk[3][5];
	WORD field_24A2[5];
	WORD ado_pos2[5];
	DWORD field_24B6[5][30];
	WORD field_270E[5];
	int field_2718;
	int field_271C;
	__int16 field_2720;
	WORD wait_tbl2[5];
	int field_272C;
	int field_2730;
	__int16 field_2734;
	WORD field_2736[5];
	DWORD wait_tbl[5];
	char sample_names[4][80];
	WORD sample_usage[4];
	WORD sample_index;
	DWORD field_289E;
	__int16 field_28A2;
	__int16 field_28A4;
	__int16 field_28A6;
	WORD rect_index;
	__int16 field_28AA;
	__int16 rx;
	WORD rect_xy_index[5];
	WORD field_28B8[5];
	__int16 field_28C2;
	int field_28C4;
	int field_28C8;
	WORD field_28CC[5];
	__int16 field_28D6;
	int field_28D8;
	int field_28DC;
	WORD field_28E0[1];
	__int16 field_28E2;
	int field_28E4;
	WORD field_28E8[1];
	__int16 field_28EA;
	int field_28EC;
	int field_28F0;
	int field_28F4;
	int field_28F8;
	__int16 field_28FC;
	WORD field_28FE[5];
	WORD field_2908[1];
	__int16 field_290A;
	int field_290C;
	char field_2910;
	char field_2911;
	DWORD field_2912[1];
	__int16 field_2916;
	int field_2918;
	int field_291C;
	int field_2920;
	__int16 field_2924;
	DWORD field_2926[1];
	__int16 field_292A;
	int field_292C;
	int field_2930;
	int field_2934;
	__int16 field_2938;
	WORD ent_index[1];
	int field_293C;
	int field_2940;
	int msg_enabled;
	DWORD field_2948[5];
	WORD msg_pos[5];
	WORD wait_timer[5];
	char msg_buf[5][512];
	WORD field_3370[5];
	CTexture* bank_no[21];
	WORD bank_spr_id[21];
	LPCVOID field_33F8[9];
	short scroll_x;
	short scroll_y;
	short field_3420;
	short scrl_x;
	short scrl_y;
	short field_3426;
	WORD map_left;
	WORD map_top;
	__int16 field_342C;
	WORD map_right;
	WORD map_bottom;
	__int16 field_3432;
	WORD field_3434;
	WORD texcol_r;
	WORD texcol_g;
	WORD texcol_b;
	WORD bkcol_r;
	WORD bkcol_g;
	WORD bkcol_b;
	WORD msg_base_wait;
	__int16 field_3444;
	WORD field_3446[1];
	int field_3448;
	int field_344C;
	int field_3450;
	int field_3454;
	int field_3458;
	int field_345C;
	int field_3460;
	int field_3464;
	int field_3468;
	int field_346C;
	int field_3470;
	int field_3474;
	int field_3478;
	int field_347C;
	__int16 field_3480;
	WORD field_3482[1];
	int field_3484;
	int field_3488;
	int field_348C;
	int field_3490;
	int field_3494;
	int field_3498;
	int field_349C;
	int field_34A0;
	int field_34A4;
	int field_34A8;
	int field_34AC;
	int field_34B0;
	int field_34B4;
	int field_34B8;
	char field_34BC;
	char field_34BD;
	WORD size_tbl[105];
	int field_3598;
	int field_359C;
	int field_35A0;
	int field_35A4;
	int field_35A8;
	int field_35AC;
	int field_35B0;
	int field_35B4;
	int field_35B8;
	int field_35BC;
	__int16 field_35C0;
	__int16 field_35C2;
};

struct VM_Data
{
	WORD vm_usage[5],
		vm_evt_pos,
		vm_index0[500],
		vm_var0,
		vm_index1[30],
		vm_index2[512],
		vm_index3[64],
		vm_index4[64],
		vm_index5[128],
		vm_index6[530];
	CRect vm_rects[30];
	WORD vm_count_index,
		render_x,
		render_y,
		render_w,
		render_h,
		word_42694A,
		word_42694C,
		word_42694E,
		word_426950[12];
};

//extern WORD vm_data.vm_usage[5],
//	vm_data.vm_evt_pos,
//	vm_data.vm_index0[500],
//	vm_data.vm_var0,
//	vm_data.vm_index1[30],
//	vm_data.vm_index2[512],
//	vm_data.vm_index3[64],
//	vm_data.vm_index4[64],
//	vm_data.vm_index5[128],
//	//vm_data.vm_index6[500],
//	//vm_index7[30];
//	vm_data.vm_index6[530];

//extern CRect vm_rects[30];
extern int error_no;
extern VM_Data vm_data;

void vm_func7();

int WriteData();

void Vm_load(VM* vm);

void Vm_update(VM* vm, int mode);

void Vm_screen_off();
void Vm_work_clr();
void Vm_set_proc(WORD p);
void Vm_pal_set(int type, int index, int count, int delta, WORD id);

int Vm_mark_event(WORD index, WORD a2);
void Vm_set_63();

void Vm_all_spr_disp();
int Vm_ent_wait(int id);

int __cdecl game_state_get(int a1);
