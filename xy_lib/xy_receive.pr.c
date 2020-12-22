/* Process model C form file: xy_receive.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char xy_receive_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A op_runsim 7 5F97FD16 5F97FD16 1 LAPTOP-C9HA4977 m1829 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                  ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

typedef struct
{
	
	char ch[1000];

} xy_pk_fm;

xy_pk_fm p;
xy_pk_fm *q=&p;

int i,pk_len,len;

#define DATA_INTRPT	(op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == 0)

/* End of Header Block */

#if !defined (VOSD_NO_FIN)
#undef	BIN
#undef	BOUT
#define	BIN		FIN_LOCAL_FIELD(_op_last_line_passed) = __LINE__ - _op_block_origin;
#define	BOUT	BIN
#define	BINIT	FIN_LOCAL_FIELD(_op_last_line_passed) = 0; _op_block_origin = __LINE__;
#else
#define	BINIT
#endif /* #if !defined (VOSD_NO_FIN) */



/* State variable definitions */
typedef struct
	{
	/* Internal state tracking for FSM */
	FSM_SYS_STATE
	/* State Variables */
	Objid	                  		own_id                                          ;
	char	                   		name[20]                                        ;
	Objid	                  		own_node_id                                     ;
	Prohandle	              		own_prohandle                                   ;
	} xy_receive_state;

#define own_id                  		op_sv_ptr->own_id
#define name                    		op_sv_ptr->name
#define own_node_id             		op_sv_ptr->own_node_id
#define own_prohandle           		op_sv_ptr->own_prohandle

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	xy_receive_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((xy_receive_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* No Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ };
#endif

/* Undefine optional tracing in FIN/FOUT/FRET */
/* The FSM has its own tracing code and the other */
/* functions should not have any tracing.		  */
#undef FIN_TRACING
#define FIN_TRACING

#undef FOUTRET_TRACING
#define FOUTRET_TRACING

#if defined (__cplusplus)
extern "C" {
#endif
	void xy_receive (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_xy_receive_init (int * init_block_ptr);
	void _op_xy_receive_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_xy_receive_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_xy_receive_alloc (VosT_Obtype, int);
	void _op_xy_receive_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
xy_receive (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (xy_receive ());

		{
		/* Temporary Variables */
		//int key=1000,port,local_port;
		
		//Objid own_mod_objid;
		//char proc_model_name[20];
		//Packet* pk_ptr;
		Packet* xy_pk_ptr;
		/* End of Temporary Variables */


		FSM_ENTER_NO_VARS ("xy_receive")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (st_0) enter executives **/
			FSM_STATE_ENTER_UNFORCED_NOLABEL (0, "st_0", "xy_receive [st_0 enter execs]")
				FSM_PROFILE_SECTION_IN ("xy_receive [st_0 enter execs]", state0_enter_exec)
				{
				 
				
				//获取模块ID
				own_id= op_id_self();
					
					
				//获取进程的名字
				op_ima_obj_attr_get(own_id, "name", name);
				
				//调度自中断，等待底层初始化完成
				op_intrpt_schedule_self(op_sim_time(), 0);
				
				
				
				
				
				
				
				
				/*
				own_mod_objid = op_id_self ();
				op_ima_obj_attr_get (own_mod_objid, "name", proc_model_name);
				if(strcmp("p_12",proc_model_name) == 0){
					local_port = 2000;
				}
				if(strcmp("p_13",proc_model_name) == 0){
					local_port = 3000;
				}
				*/
				
				//xy_pk_ptr = op_pk_get(0);
				
				//op_pk_print(xy_pk_ptr);
				
				//op_pk_send(xy_pk_ptr,2);
				
				//op_pk_print(xy_pk_ptr);
				//op_pk_nfd_get(xy_pk_ptr, "port",&port);
				//op_pk_nfd_get(xy_pk_ptr, "pk",&pk_ptr);
				
				/*
				if(local_port == port){
					printf("%s receive %d packet.\n",proc_model_name,port);
					pk_len = op_pk_total_size_get (pk_ptr) ;
					op_pk_fd_get(pk_ptr,0,&q);
					for(i=0;i*8<pk_len;i++){
					
					for(len=128;len >0;  len = len >> 1){
						if(q->ch[i] & len)
								printf("1");
						else
							printf("0");
					}
						printf("_");	
					}
					printf("\n");
					
				}
				*/
				
				
				
				
				
				
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (1,"xy_receive")


			/** state (st_0) exit executives **/
			FSM_STATE_EXIT_UNFORCED (0, "st_0", "xy_receive [st_0 exit execs]")


			/** state (st_0) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "st_0", "st_1", "tr_9", "xy_receive [st_0 -> st_1 : default / ]")
				/*---------------------------------------------------------*/



			/** state (st_1) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "st_1", state1_enter_exec, "xy_receive [st_1 enter execs]")
				FSM_PROFILE_SECTION_IN ("xy_receive [st_1 enter execs]", state1_enter_exec)
				{
				
				
				
				xy_pk_ptr = op_pk_get(0);
				
				if(xy_pk_ptr != OPC_NIL){
				    printf("%s reseive packet, send to external.\n",name);
				    op_pk_send(xy_pk_ptr,0);
				
				}else{
					printf("packet is null.\n");
				
				}
				
				}
				FSM_PROFILE_SECTION_OUT (state1_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"xy_receive")


			/** state (st_1) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "st_1", "xy_receive [st_1 exit execs]")


			/** state (st_1) transition processing **/
			FSM_TRANSIT_ONLY ((DATA_INTRPT), 1, state1_enter_exec, ;, st_1, "DATA_INTRPT", "", "st_1", "st_1", "tr_2", "xy_receive [st_1 -> st_1 : DATA_INTRPT / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"xy_receive")
		}
	}




void
_op_xy_receive_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_xy_receive_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_xy_receive_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_xy_receive_svar function. */
#undef own_id
#undef name
#undef own_node_id
#undef own_prohandle

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_xy_receive_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_xy_receive_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (xy_receive)",
		sizeof (xy_receive_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_xy_receive_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	xy_receive_state * ptr;
	FIN_MT (_op_xy_receive_alloc (obtype))

	ptr = (xy_receive_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "xy_receive [st_0 enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_xy_receive_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	xy_receive_state		*prs_ptr;

	FIN_MT (_op_xy_receive_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (xy_receive_state *)gen_ptr;

	if (strcmp ("own_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->own_id);
		FOUT
		}
	if (strcmp ("name" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->name);
		FOUT
		}
	if (strcmp ("own_node_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->own_node_id);
		FOUT
		}
	if (strcmp ("own_prohandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->own_prohandle);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

