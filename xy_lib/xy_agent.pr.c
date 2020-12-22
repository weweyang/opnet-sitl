/* Process model C form file: xy_agent.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char xy_agent_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A modeler 7 5F980E84 5F980E84 1 LAPTOP-C9HA4977 m1829 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                    ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

//引用头文件
#include <ip_addr_v4.h>
#include <ip_rte_v4.h>
#include <oms_pr.h>
#include <oms_tan.h>
#include <ip_dgram_sup.h>
#include <ip_mcast_support.h>
#include <udp_api.h>



//定义状态转移条件
#define FROM_EXTERNAL_DATA_INTRPT	(intrpt_type == OPC_INTRPT_STRM && intrpt_strm == input_strm)
#define TO_EXTERNAL_DATA_INTRPT		(intrpt_type == OPC_INTRPT_STRM && intrpt_strm != input_strm)




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
	Objid	                  		own_node_id                                     ;
	Ici *	                  		app_ici_ptr                                     ;
	int	                    		input_strm                                      ;
	int	                    		output_strm                                     ;
	int	                    		send_iface                                      ;
	int	                    		own_label                                       ;
	int	                    		local_port                                      ;
	int	                    		rem_port                                        ;
	InetT_Address*	         		rem_addr_ptr                                    ;
	InetT_Address*	         		intf_addr_ptr                                   ;
	int	                    		incoming_intf_index                             ;
	int	                    		type_of_service                                 ;
	Ici *	                  		ici_ptr                                         ;
	Prohandle	              		own_prohandle                                   ;
	char	                   		proc_model_name[20]                             ;
	} xy_agent_state;

#define own_id                  		op_sv_ptr->own_id
#define own_node_id             		op_sv_ptr->own_node_id
#define app_ici_ptr             		op_sv_ptr->app_ici_ptr
#define input_strm              		op_sv_ptr->input_strm
#define output_strm             		op_sv_ptr->output_strm
#define send_iface              		op_sv_ptr->send_iface
#define own_label               		op_sv_ptr->own_label
#define local_port              		op_sv_ptr->local_port
#define rem_port                		op_sv_ptr->rem_port
#define rem_addr_ptr            		op_sv_ptr->rem_addr_ptr
#define intf_addr_ptr           		op_sv_ptr->intf_addr_ptr
#define incoming_intf_index     		op_sv_ptr->incoming_intf_index
#define type_of_service         		op_sv_ptr->type_of_service
#define ici_ptr                 		op_sv_ptr->ici_ptr
#define own_prohandle           		op_sv_ptr->own_prohandle
#define proc_model_name         		op_sv_ptr->proc_model_name

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	xy_agent_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((xy_agent_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


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
	void xy_agent (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_xy_agent_init (int * init_block_ptr);
	void _op_xy_agent_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_xy_agent_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_xy_agent_alloc (VosT_Obtype, int);
	void _op_xy_agent_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
xy_agent (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (xy_agent ());

		{
		/* Temporary Variables */
		int				intrpt_type;
		int				intrpt_strm;
		int				intrpt_code;
		
		Packet * 			app_data;
		
		List*			proc_record_handle_list_ptr;
		OmsT_Pr_Handle	process_record_handle;
		Objid			neighbor_mod_objid;
		
		//List * 				proc_record_handle_list_ptr;
		//OmsT_Pr_Handle 		process_record_handle;
		//int					proc_record_handle_list_size;
		
		OmsT_Pr_Handle my_proc_handle;
		
		/* End of Temporary Variables */


		FSM_ENTER ("xy_agent")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_UNFORCED_NOLABEL (0, "init", "xy_agent [init enter execs]")
				FSM_PROFILE_SECTION_IN ("xy_agent [init enter execs]", state0_enter_exec)
				{
					//获取模块ID
					own_id= op_id_self();
					
					//获取节点ID
					own_node_id = op_topo_parent(own_id);
					
					//obtain process handle
					own_prohandle = op_pro_self();
					
					//获取进程的名字
					op_ima_obj_attr_get(own_id, "process model", proc_model_name);
				
					//注册进程
					my_proc_handle = oms_pr_process_register(own_node_id,own_id,own_prohandle, proc_model_name);
				
					//设置进程的相关参数
					oms_pr_attr_set(my_proc_handle,
									"protocol", 		OMSC_PR_STRING, "external_app",
									OPC_NIL);
					
					app_ici_ptr = op_ici_create("udp_command_inet");
						
					//调度自中断，等待底层初始化完成
				    op_intrpt_schedule_self(op_sim_time(), 0);
					
					
				
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (1,"xy_agent")


			/** state (init) exit executives **/
			FSM_STATE_EXIT_UNFORCED (0, "init", "xy_agent [init exit execs]")


			/** state (init) transition processing **/
			FSM_TRANSIT_FORCE (4, state4_enter_exec, ;, "default", "", "init", "wait_0", "tr_13", "xy_agent [init -> wait_0 : default / ]")
				/*---------------------------------------------------------*/



			/** state (idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "idle", state1_enter_exec, "xy_agent [idle enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"xy_agent")


			/** state (idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "idle", "xy_agent [idle exit execs]")
				FSM_PROFILE_SECTION_IN ("xy_agent [idle exit execs]", state1_exit_exec)
				{
				intrpt_type = op_intrpt_type ();
				if (intrpt_type == OPC_INTRPT_STRM)
					intrpt_strm = op_intrpt_strm ();
				else
					intrpt_code = op_intrpt_code ();
				
				
				
				
				
				}
				FSM_PROFILE_SECTION_OUT (state1_exit_exec)


			/** state (idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("xy_agent [idle trans conditions]", state1_trans_conds)
			FSM_INIT_COND (FROM_EXTERNAL_DATA_INTRPT)
			FSM_TEST_COND (TO_EXTERNAL_DATA_INTRPT)
			FSM_TEST_LOGIC ("idle")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 2, state2_enter_exec, ;, "FROM_EXTERNAL_DATA_INTRPT", "", "idle", "send", "tr_1", "xy_agent [idle -> send : FROM_EXTERNAL_DATA_INTRPT / ]")
				FSM_CASE_TRANSIT (1, 3, state3_enter_exec, ;, "TO_EXTERNAL_DATA_INTRPT", "", "idle", "receive", "tr_4", "xy_agent [idle -> receive : TO_EXTERNAL_DATA_INTRPT / ]")
				}
				/*---------------------------------------------------------*/



			/** state (send) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "send", state2_enter_exec, "xy_agent [send enter execs]")
				FSM_PROFILE_SECTION_IN ("xy_agent [send enter execs]", state2_enter_exec)
				{
				//from external
				
				
				ici_ptr = op_intrpt_ici ();
				app_data = op_pk_get(intrpt_strm);
				
				op_ici_attr_get(ici_ptr, "rem_addr",&rem_addr_ptr);
				
				printf("addr:%d.\n", rem_addr_ptr->address.ipv4_addr);
				//192.168.0.1 = -1062731775  192.168.0.120 = -1062731656 192.168.0.3=-1062731773
				
				rem_addr_ptr->address.ipv4_addr= -1062731773;
				
				
				op_ici_attr_get(ici_ptr, "rem_port",&rem_port);
				op_ici_attr_get(ici_ptr, "local_port",&local_port);
				
				//op_ici_attr_get (ici_ptr, "interface_received", &intf_addr_ptr);
				//op_ici_attr_get (ici_ptr, "incoming_intf_index", &incoming_intf_index);
				//op_ici_attr_get (ici_ptr, "Type of Service", &type_of_service); 
				
				switch(local_port){
					case 2000:
						op_pk_send(app_data,1);
						printf("external: local port:%d,send to p_12.\n",local_port);
						break;
					case 3000:
						printf("external: local port:%d,send to p_13.\n",local_port);
						op_pk_send(app_data,2);
						break;
					default:
					 	printf("external:error local port:%d,destroy the packet.\n",local_port);
						op_pk_destroy(app_data);
					
						
				
				}
				
				
				
				
				
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (send) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "send", "xy_agent [send exit execs]")


			/** state (send) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "send", "idle", "tr_2", "xy_agent [send -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (receive) enter executives **/
			FSM_STATE_ENTER_FORCED (3, "receive", state3_enter_exec, "xy_agent [receive enter execs]")
				FSM_PROFILE_SECTION_IN ("xy_agent [receive enter execs]", state3_enter_exec)
				{
				//from opnet
				
				//从流中断端口中提取应用数据
				app_data = op_pk_get(intrpt_strm);	
				
				op_ici_attr_set(app_ici_ptr, "strm_index", input_strm);
				op_ici_attr_set (app_ici_ptr, "rem_addr", rem_addr_ptr);	 
				op_ici_attr_set (app_ici_ptr, "rem_port", rem_port);
				op_ici_attr_set (app_ici_ptr, "local_port", local_port);
				
				//op_ici_attr_set (app_ici_ptr, "interface received", intf_addr_ptr);
				//op_ici_attr_set (app_ici_ptr, "Type of Service", type_of_service); 
				//op_ici_attr_set (app_ici_ptr, "incoming_intf_index", incoming_intf_index);
					
				
				//op_ici_print(app_ici_ptr);
				
				
				op_ici_install(app_ici_ptr);
				op_pk_send(app_data,output_strm);
				
				
				
				
				
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** state (receive) exit executives **/
			FSM_STATE_EXIT_FORCED (3, "receive", "xy_agent [receive exit execs]")


			/** state (receive) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "receive", "idle", "tr_6", "xy_agent [receive -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (wait_0) enter executives **/
			FSM_STATE_ENTER_UNFORCED (4, "wait_0", state4_enter_exec, "xy_agent [wait_0 enter execs]")
				FSM_PROFILE_SECTION_IN ("xy_agent [wait_0 enter execs]", state4_enter_exec)
				{
				//调度自中断，等待底层初始化完成
				op_intrpt_schedule_self(op_sim_time(), 0);
				}
				FSM_PROFILE_SECTION_OUT (state4_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (9,"xy_agent")


			/** state (wait_0) exit executives **/
			FSM_STATE_EXIT_UNFORCED (4, "wait_0", "xy_agent [wait_0 exit execs]")
				FSM_PROFILE_SECTION_IN ("xy_agent [wait_0 exit execs]", state4_exit_exec)
				{
				/* Find the stream index connect this module to lower layer	*/
				/* module - which is external_app. Obtain the process registry for  	*/
				/* the external_app process model in this node						*/
				proc_record_handle_list_ptr = op_prg_list_create ();
				oms_pr_process_discover (OPC_OBJID_INVALID, proc_record_handle_list_ptr,
					"node objid",	OMSC_PR_OBJID,		own_node_id,
					"protocol",		OMSC_PR_STRING,		"udp",
					OPC_NIL);
				
				/* Assume there is only one udp module in the node 	*/
				/* Get the process record handle for the external_app process model	*/
				process_record_handle = (OmsT_Pr_Handle) op_prg_list_access (proc_record_handle_list_ptr, OPC_LISTPOS_HEAD);
				
				/* Obtain the module object id of the udp.				*/
				oms_pr_attr_get (process_record_handle, "module objid", OMSC_PR_OBJID, &neighbor_mod_objid);
						
				/* Determine the input and output stream indices.			*/
				oms_tan_neighbor_streams_find (own_id, neighbor_mod_objid, &input_strm, &output_strm);
				
				//printf("external input strm:%d output strm:%d.\n",input_strm,output_strm);
				
				/* Deallocate the registry list pointer.					*/
				while (op_prg_list_size (proc_record_handle_list_ptr))
					{
					op_prg_list_remove (proc_record_handle_list_ptr, OPC_LISTPOS_HEAD);
					}
				op_prg_mem_free (proc_record_handle_list_ptr);
				}
				FSM_PROFILE_SECTION_OUT (state4_exit_exec)


			/** state (wait_0) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "wait_0", "idle", "tr_12", "xy_agent [wait_0 -> idle : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"xy_agent")
		}
	}




void
_op_xy_agent_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
	/* No Diagnostic Block */
	}




void
_op_xy_agent_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_xy_agent_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_xy_agent_svar function. */
#undef own_id
#undef own_node_id
#undef app_ici_ptr
#undef input_strm
#undef output_strm
#undef send_iface
#undef own_label
#undef local_port
#undef rem_port
#undef rem_addr_ptr
#undef intf_addr_ptr
#undef incoming_intf_index
#undef type_of_service
#undef ici_ptr
#undef own_prohandle
#undef proc_model_name

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_xy_agent_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_xy_agent_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (xy_agent)",
		sizeof (xy_agent_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_xy_agent_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	xy_agent_state * ptr;
	FIN_MT (_op_xy_agent_alloc (obtype))

	ptr = (xy_agent_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "xy_agent [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_xy_agent_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	xy_agent_state		*prs_ptr;

	FIN_MT (_op_xy_agent_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (xy_agent_state *)gen_ptr;

	if (strcmp ("own_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->own_id);
		FOUT
		}
	if (strcmp ("own_node_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->own_node_id);
		FOUT
		}
	if (strcmp ("app_ici_ptr" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->app_ici_ptr);
		FOUT
		}
	if (strcmp ("input_strm" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->input_strm);
		FOUT
		}
	if (strcmp ("output_strm" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->output_strm);
		FOUT
		}
	if (strcmp ("send_iface" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->send_iface);
		FOUT
		}
	if (strcmp ("own_label" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->own_label);
		FOUT
		}
	if (strcmp ("local_port" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->local_port);
		FOUT
		}
	if (strcmp ("rem_port" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->rem_port);
		FOUT
		}
	if (strcmp ("rem_addr_ptr" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->rem_addr_ptr);
		FOUT
		}
	if (strcmp ("intf_addr_ptr" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->intf_addr_ptr);
		FOUT
		}
	if (strcmp ("incoming_intf_index" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->incoming_intf_index);
		FOUT
		}
	if (strcmp ("type_of_service" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->type_of_service);
		FOUT
		}
	if (strcmp ("ici_ptr" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->ici_ptr);
		FOUT
		}
	if (strcmp ("own_prohandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->own_prohandle);
		FOUT
		}
	if (strcmp ("proc_model_name" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->proc_model_name);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

