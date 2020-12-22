/* Process model C form file: xy_udp.pr.c */
/* Portions of this file copyright 1986-2008 by OPNET Technologies, Inc. */



/* This variable carries the header into the object file */
const char xy_udp_pr_c [] = "MIL_3_Tfile_Hdr_ 145A 30A op_runsim 7 5F97FD16 5F97FD16 1 LAPTOP-C9HA4977 m1829 0 0 none none 0 0 none 0 0 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                                  ";
#include <string.h>



/* OPNET system definitions */
#include <opnet.h>



/* Header Block */

#include <udp_api.h>
#include <oms_pr.h>
#include <oms_tan.h>
#include <ip_addr_v4.h>
#include <llm_support.h>
#include <ip_cmn_rte_table.h>
#include <ip_with_rip_v3.h>
#include <rip_notif_log_support.h>
#include <udp_dgram_sup.h>
#include <ip_higher_layer_proto_reg_sup.h>

/* Stream indices for interface to network layer. */
#define UDPC_INSTRM_NETWORK				0
#define UDPC_OUTSTRM_NETWORK			0

/***** Macros *****/

/***** Typedefs *****/

/* UdpT_Tcb : Transmission Control Block */
typedef struct
	{
	/* Application and connection identification information. */
	Objid				app_objid;
	int					strm_index;

	/* Socket information. */
	UdpT_Port			local_port;

	/* Whether or not the application supports the			*/
	/* InetT_Address strucure.								*/
	Boolean				inet_address_supported;

	} UdpT_Tcb;

/* Transition macros */
#define CREATE			((intrpt_type == OPC_INTRPT_REMOTE) &&			\
						(intrpt_code == UDPC_COMMAND_CREATE_PORT))
#define SEND			((intrpt_type == OPC_INTRPT_STRM) && 			\
						(intrpt_strm !=	input_strm))
#define DGRAM_ARRIVAL	((intrpt_type == OPC_INTRPT_STRM) &&			\
						(intrpt_strm == input_strm))

static UdpT_Tcb*	udp_tcb_from_port ();
static void			udp_lan_handle_get ();
static void			udp_warn (const char* msg0, const char* msg1, const char* msg2);




//xy
typedef struct
{
	
	char ch[6];

} xy_pk_fm;

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
	List*	                  		tcb_list                                        ;
	Ici*	                   		net_ici_ptr                                     ;
	Ici*	                   		app_ici_v3_ptr                                  ;
	int	                    		udp_trace_active                                ;
	int	                    		port_id_new                                     ;
	Objid	                  		own_mod_objid                                   ;
	Objid	                  		own_node_objid                                  ;
	Prohandle	              		own_prohandle                                   ;
	OmsT_Pr_Handle	         		own_process_record_handle                       ;
	char	                   		proc_model_name[20]                             ;
	LlmT_Lan_Handle	        		my_lanhandle                                    ;	/* This variables are used to record information */
	                        		                                                	/* for lan level models.                         */
	Boolean	                		lan_node                                        ;
	int	                    		sess_svr_id                                     ;
	int	                    		sess_wkstn_id                                   ;
	int	                    		input_strm                                      ;	/* Input stream from network layer */
	int	                    		output_strm                                     ;	/* Output stream index to network layer */
	Stathandle	             		sent_packet_sec_handle                          ;	/* Used for collection of packets sent per second statistic */
	Stathandle	             		sent_byte_sec_handle                            ;	/* Used for collection of bytes sent per second statistic */
	Stathandle	             		receive_packet_sec_handle                       ;	/* Used for collection of packets received per second statistic */
	Stathandle	             		receive_byte_sec_handle                         ;	/* Used for collection of bytes received per second statistic */
	Ici*	                   		app_ici_v4_ptr                                  ;	/* Pointer to an ici of type udp_command_v4 */
	} xy_udp_state;

#define tcb_list                		op_sv_ptr->tcb_list
#define net_ici_ptr             		op_sv_ptr->net_ici_ptr
#define app_ici_v3_ptr          		op_sv_ptr->app_ici_v3_ptr
#define udp_trace_active        		op_sv_ptr->udp_trace_active
#define port_id_new             		op_sv_ptr->port_id_new
#define own_mod_objid           		op_sv_ptr->own_mod_objid
#define own_node_objid          		op_sv_ptr->own_node_objid
#define own_prohandle           		op_sv_ptr->own_prohandle
#define own_process_record_handle		op_sv_ptr->own_process_record_handle
#define proc_model_name         		op_sv_ptr->proc_model_name
#define my_lanhandle            		op_sv_ptr->my_lanhandle
#define lan_node                		op_sv_ptr->lan_node
#define sess_svr_id             		op_sv_ptr->sess_svr_id
#define sess_wkstn_id           		op_sv_ptr->sess_wkstn_id
#define input_strm              		op_sv_ptr->input_strm
#define output_strm             		op_sv_ptr->output_strm
#define sent_packet_sec_handle  		op_sv_ptr->sent_packet_sec_handle
#define sent_byte_sec_handle    		op_sv_ptr->sent_byte_sec_handle
#define receive_packet_sec_handle		op_sv_ptr->receive_packet_sec_handle
#define receive_byte_sec_handle 		op_sv_ptr->receive_byte_sec_handle
#define app_ici_v4_ptr          		op_sv_ptr->app_ici_v4_ptr

/* These macro definitions will define a local variable called	*/
/* "op_sv_ptr" in each function containing a FIN statement.	*/
/* This variable points to the state variable data structure,	*/
/* and can be used from a C debugger to display their values.	*/
#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE
#define FIN_PREAMBLE_DEC	xy_udp_state *op_sv_ptr;
#define FIN_PREAMBLE_CODE	\
		op_sv_ptr = ((xy_udp_state *)(OP_SIM_CONTEXT_PTR->_op_mod_state_ptr));


/* Function Block */

#if !defined (VOSD_NO_FIN)
enum { _op_block_origin = __LINE__ + 2};
#endif

static UdpT_Tcb*
udp_tcb_from_port (UdpT_Port loc_port) 
	{
	UdpT_Tcb*			tcb_ptr;
	int					i, list_size;

	/** Find the TCB with the given port number			**/
	/** in the TCB list.  Returns a pointer to the TCB 	**/
	/** record, or NIL if no match is found.			**/
	FIN (udp_tcb_from_port (loc_port))

	list_size = op_prg_list_size (tcb_list);
	for (i = 0; i < list_size; i++)
		{
		tcb_ptr = (UdpT_Tcb *) op_prg_list_access (tcb_list, i);
		if (loc_port == tcb_ptr->local_port)
			{
			break;
			}
		}
	if (i == list_size)
		tcb_ptr = OPC_NIL;

	FRET (tcb_ptr)
	}

static void
udp_lan_handle_get ()
	{
	List*			proc_record_handle_list_ptr;
	int				record_handle_list_size;
	OmsT_Pr_Handle	proc_record_handle;
	double			server_id;

	/**	This function obtains lan handle if it is a LAN node.	**/
	FIN (udp_lan_handle_get ());

	/*	Find out the node type from model wide process registry */
	proc_record_handle_list_ptr = op_prg_list_create ();

	oms_pr_process_discover (OPC_OBJID_INVALID, proc_record_handle_list_ptr, 
			"node objid",	OMSC_PR_OBJID,		own_node_objid, 
			"node_type", 	OMSC_PR_STRING, 	"lan_mac", 
			OPC_NIL);

	/*	Obtain the list size of the discovered processes.		*/
	record_handle_list_size = op_prg_list_size (proc_record_handle_list_ptr);
	
	if (record_handle_list_size == 1)
		{
		/*	Set a flag indicating that this is a LAN node.		*/
		lan_node = OPC_TRUE;

		/*	Obtain total number of workstations in this node.	*/
		/*	This is equal to the server id.						*/
		proc_record_handle = 
			(OmsT_Pr_Handle) op_prg_list_access (proc_record_handle_list_ptr, OPC_LISTPOS_HEAD);
		
		oms_pr_attr_get (proc_record_handle, "wkstn count", OMSC_PR_NUMBER, &server_id);
		
		sess_svr_id = (int) server_id;

		/*	Obtain lan handle from llm package.					*/
		my_lanhandle = llm_lan_handle_get (own_node_objid);
		}
	
	/* 	Deallocate no longer needed process registry 		*/
	/*	information.										*/
	while (op_prg_list_size (proc_record_handle_list_ptr))
		op_prg_list_remove (proc_record_handle_list_ptr, OPC_LISTPOS_HEAD);
	
	op_prg_mem_free (proc_record_handle_list_ptr);
	
	FOUT;
	}

static void
udp_warn (const char * msg0, const char * msg1, const char * msg2)
	{
	/** Print a warning message and resume. **/
	FIN (udp_warn (msg0, msg1, msg2));

	op_prg_odb_print_major ("Warning from UDP socket process (rip_udp_v3):",
		msg0, msg1, msg2, OPC_NIL);

	FOUT;
	}

/* End of Function Block */

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
	void xy_udp (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Obtype _op_xy_udp_init (int * init_block_ptr);
	void _op_xy_udp_diag (OP_SIM_CONTEXT_ARG_OPT);
	void _op_xy_udp_terminate (OP_SIM_CONTEXT_ARG_OPT);
	VosT_Address _op_xy_udp_alloc (VosT_Obtype, int);
	void _op_xy_udp_svar (void *, const char *, void **);


#if defined (__cplusplus)
} /* end of 'extern "C"' */
#endif




/* Process model interrupt handling procedure */


void
xy_udp (OP_SIM_CONTEXT_ARG_OPT)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	FIN_MT (xy_udp ());

		{
		/* Temporary Variables */
		int				intrpt_type;
		int				intrpt_strm;
		int				intrpt_code;
		Ici*			ici_ptr;
		Ici*			app_ici_ptr;
		Objid			strm_objid;
		
		UdpT_Port		local_port;
		int				local_minor_port;
		IpT_Address		rem_ipv4_addr;
		IpT_Address		ipv4_intf_addr;
		InetT_Address*	rem_addr_ptr;
		InetT_Address*	intf_addr_ptr;
		InetT_Address	temp_ip_addr;
		InetT_Address	intf_addr;
		int				type_of_service; 
		int				incoming_intf_index;
		
		UdpT_Port		rem_port;
		int				strm_index;
		
		UdpT_Tcb*		tcb_ptr;
		
		char			msg0 [512];
		
		int				port_assigned;
		
		Packet*			pkptr;
		Packet*			data_pkptr;
		
		
		int					conn_class;
		
		int				higher_layer_protocol_type;
		int				intf_num;
		/* used for discover lower layer module ip_encap */
		List*			proc_record_handle_list_ptr;
		OmsT_Pr_Handle	process_record_handle;
		Objid			neighbor_mod_objid;
		
		/* used for collection of UDP statistics */
		double pk_size;
		
		int				inet_address_supported;
		
		OpT_Int64		vmac_addr = -1;
		
		
		// xy
		
		int pk_len;
		//Packet* xy_pk_ptr;
		Packet*  xy_p;
		xy_pk_fm p;
		xy_pk_fm *q=&p;
		UdpT_Tcb* xy_tcb_ptr[20];
		//char iciname[100];
		
		/* End of Temporary Variables */


		FSM_ENTER ("xy_udp")

		FSM_BLOCK_SWITCH
			{
			/*---------------------------------------------------------*/
			/** state (init) enter executives **/
			FSM_STATE_ENTER_UNFORCED_NOLABEL (0, "init", "xy_udp [init enter execs]")
				FSM_PROFILE_SECTION_IN ("xy_udp [init enter execs]", state0_enter_exec)
				{
				
				/* Initialize the simulation notification log.	*/
				rip_notification_log_init ();
				
				tcb_list = op_prg_list_create ();
				
				net_ici_ptr = op_ici_create ("inet_encap_req");
				app_ici_v3_ptr = op_ici_create ("udp_command_v3");
				app_ici_v4_ptr = op_ici_create ("udp_command_inet");
				
				/* when assigning ports, start at lowest unreserved port */
				port_id_new = 255;
				
				/* Obtain the udp module's objid. */
				own_mod_objid = op_id_self ();
				
				/* Obtain the node's objid. */
				own_node_objid = op_topo_parent (own_mod_objid);
				
				/* Obtain the udp process's prohandle. */
				own_prohandle = op_pro_self ();
				
				/* Obtain the name of the process. It is the "process model" attribute of the module. */
				op_ima_obj_attr_get (own_mod_objid, "process model", proc_model_name);
				
				/* Register the process in the model-wide registry. */
				own_process_record_handle = (OmsT_Pr_Handle) oms_pr_process_register (own_node_objid, own_mod_objid, 
											own_prohandle, proc_model_name);
				
				/* Register the protocol attribute in the registry. */
				oms_pr_attr_set (own_process_record_handle, "protocol", OMSC_PR_STRING, "udp", OPC_NIL);
				
				/** Register this higher layer protocol with the label	**/
				/** "udp" and id IpC_Protocol_Udp.						**/
				
				/* Set the protocol type.							*/
				higher_layer_protocol_type = IpC_Protocol_Udp;
				
				/* Register this higher layer protocol label with	*/
				/* the given higher layer protocol type.			*/
				Inet_Higher_Layer_Protocol_Register ("udp", &higher_layer_protocol_type);
				
				/*	Initialize variable used to store lan level 	*/
				/*	model information.								*/
				lan_node 			= OPC_FALSE;
				sess_wkstn_id		= LlmC_Unspec_Wkstn_Id;
				sess_svr_id			= LlmC_Unspec_Wkstn_Id;	
				
				/* Schedule a self interrupt for ip_encap to register */
				op_intrpt_schedule_self (op_sim_time (), 0);
				
				/* Register stat handles */
				sent_packet_sec_handle		= op_stat_reg ("UDP.Traffic Sent (Packets/Sec)", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				sent_byte_sec_handle		= op_stat_reg ("UDP.Traffic Sent (Bytes/Sec)", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				receive_packet_sec_handle	= op_stat_reg ("UDP.Traffic Received (Packets/Sec)", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				receive_byte_sec_handle		= op_stat_reg ("UDP.Traffic Received (Bytes/Sec)", OPC_STAT_INDEX_NONE, OPC_STAT_LOCAL);
				
				/* Set print proc for InetT_Address fields in Icis	*/
				op_ici_format_print_proc_set ("udp_command_inet", "rem_addr",  inet_address_ici_field_print);
				}
				FSM_PROFILE_SECTION_OUT (state0_enter_exec)

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (1,"xy_udp")


			/** state (init) exit executives **/
			FSM_STATE_EXIT_UNFORCED (0, "init", "xy_udp [init exit execs]")
				FSM_PROFILE_SECTION_IN ("xy_udp [init exit execs]", state0_exit_exec)
				{
				/* Find the stream index connect this module to lower layer	*/
				/* module - which is udp. Obtain the process registry for  	*/
				/* the udp process model in this node						*/
				proc_record_handle_list_ptr = op_prg_list_create ();
				oms_pr_process_discover (OPC_OBJID_INVALID, proc_record_handle_list_ptr,
					"node objid",	OMSC_PR_OBJID,		own_node_objid,
					"protocol",		OMSC_PR_STRING,		"ip_encap",
					OPC_NIL);
				
				/* Assume there is only one ip_encap module in the node 	*/
				/* Get the process record handle for the udp process model	*/
				process_record_handle = (OmsT_Pr_Handle) op_prg_list_access (proc_record_handle_list_ptr, OPC_LISTPOS_HEAD);
				
				/* Obtain the module object id of the ip_encap.				*/
				oms_pr_attr_get (process_record_handle, "module objid", OMSC_PR_OBJID, &neighbor_mod_objid);
						
				/* Determine the input and output stream indices.			*/
				oms_tan_neighbor_streams_find (own_mod_objid, neighbor_mod_objid, &input_strm, &output_strm);
				
				printf("udp input strm:%d output strm:%d.\n",input_strm,output_strm);
				
				/* Deallocate the registry list pointer.					*/
				while (op_prg_list_size (proc_record_handle_list_ptr))
					{
					op_prg_list_remove (proc_record_handle_list_ptr, OPC_LISTPOS_HEAD);
					}
				op_prg_mem_free (proc_record_handle_list_ptr);
				}
				FSM_PROFILE_SECTION_OUT (state0_exit_exec)


			/** state (init) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "init", "idle", "tr_1", "xy_udp [init -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (idle) enter executives **/
			FSM_STATE_ENTER_UNFORCED (1, "idle", state1_enter_exec, "xy_udp [idle enter execs]")

			/** blocking after enter executives of unforced state. **/
			FSM_EXIT (3,"xy_udp")


			/** state (idle) exit executives **/
			FSM_STATE_EXIT_UNFORCED (1, "idle", "xy_udp [idle exit execs]")
				FSM_PROFILE_SECTION_IN ("xy_udp [idle exit execs]", state1_exit_exec)
				{
				/* gather information relevant to the current interrupt */   
				udp_trace_active = op_prg_odb_ltrace_active ("udp");
				
				intrpt_type = op_intrpt_type ();
				if (intrpt_type == OPC_INTRPT_STRM)
					intrpt_strm = op_intrpt_strm ();
				else
					intrpt_code = op_intrpt_code ();
				ici_ptr = op_intrpt_ici ();
				}
				FSM_PROFILE_SECTION_OUT (state1_exit_exec)


			/** state (idle) transition processing **/
			FSM_PROFILE_SECTION_IN ("xy_udp [idle trans conditions]", state1_trans_conds)
			FSM_INIT_COND (CREATE)
			FSM_TEST_COND (SEND)
			FSM_TEST_COND (DGRAM_ARRIVAL)
			FSM_TEST_LOGIC ("idle")
			FSM_PROFILE_SECTION_OUT (state1_trans_conds)

			FSM_TRANSIT_SWITCH
				{
				FSM_CASE_TRANSIT (0, 2, state2_enter_exec, ;, "CREATE", "", "idle", "CREATE", "tr_4", "xy_udp [idle -> CREATE : CREATE / ]")
				FSM_CASE_TRANSIT (1, 3, state3_enter_exec, ;, "SEND", "", "idle", "SEND", "tr_6", "xy_udp [idle -> SEND : SEND / ]")
				FSM_CASE_TRANSIT (2, 4, state4_enter_exec, ;, "DGRAM_ARRIVAL", "", "idle", "ARRIVE", "tr_16", "xy_udp [idle -> ARRIVE : DGRAM_ARRIVAL / ]")
				}
				/*---------------------------------------------------------*/



			/** state (CREATE) enter executives **/
			FSM_STATE_ENTER_FORCED (2, "CREATE", state2_enter_exec, "xy_udp [CREATE enter execs]")
				FSM_PROFILE_SECTION_IN ("xy_udp [CREATE enter execs]", state2_enter_exec)
				{
				/* Read the arguments to the CREATE_PORT command. */
				op_ici_attr_get (ici_ptr, "strm_index", &strm_index);
				op_ici_attr_get (ici_ptr, "local_port", &local_port);
				op_ici_attr_get (ici_ptr, "inet_support", &inet_address_supported);
				
				//local_port = 10024;
				//printf("111******local port: %d rem port:%d\n",local_port,rem_port);
				
				/* Find the preexisting TCB for this port, if any.				*/
				/* If none exists, create a new TCB and add it to the			*/
				/* list.  If the port id is unspecified, assign a free port.	*/
				
				if (udp_tcb_from_port (local_port) != OPC_NIL)
					{
					/* A port with the given id is already assigned. */
					/* Return an error status code to the application. */
					if (udp_trace_active)
						op_prg_odb_print_minor ("Port assignment already exists.", OPC_NIL);
				
					/* Generate a simulation log message. */
					udp_port_err_log_write (local_port);
				
					op_ici_attr_set (ici_ptr, "status", UDPC_IND_EADDRINUSE);
					}
				
				else
					{
					if (local_port == UDPC_PORT_UNSPEC)
						/* No port was specified. */
						{
						/** determine an unassigned port. **/
						port_assigned = 1;
						while (port_assigned == 1)
							{
							if (udp_tcb_from_port (port_id_new++) == OPC_NIL)
								port_assigned = 0;
							}
						local_port = port_id_new;
						}
					
				//printf("???******local port: %d rem port:%d\n",local_port,rem_port);
					
					/* If no stream index has been specified, pick a logical one. */
					if (strm_index == UDPC_STRM_INDEX_UNSPEC)
						{
						strm_objid = op_topo_connect (op_id_self (), op_intrpt_source (),
							OPC_OBJTYPE_STRM, 0);
						if (strm_objid == OPC_OBJID_INVALID)
							{
							/* Generate a simulation log message before ending the simulation. */
							udp_conf_err_log_write ();
				
							op_sim_end ("Illegal UDP CREATE_PORT command",
								"No stream connection exists from UDP process to application.", "", "");
							}
						else
							{
							op_ima_obj_attr_get (strm_objid, "src stream", &strm_index);
							op_ici_attr_set (ici_ptr, "strm_index", strm_index);
							}
						}
				
					/* This is a new port registration.			*/
					/* Create a new TCB and add it to the list. */
					tcb_ptr = (UdpT_Tcb *) op_prg_mem_alloc (sizeof (UdpT_Tcb));
					tcb_ptr->app_objid = op_intrpt_source ();
					tcb_ptr->strm_index = strm_index;
					tcb_ptr->local_port = local_port;
					tcb_ptr->inet_address_supported = inet_address_supported;
					op_prg_list_insert (tcb_list, tcb_ptr, OPC_LISTPOS_TAIL);
					
				xy_tcb_ptr[0] = (UdpT_Tcb *) op_prg_mem_alloc (sizeof (UdpT_Tcb));
				xy_tcb_ptr[0]->app_objid = op_intrpt_source ();
				xy_tcb_ptr[0]->strm_index = strm_index;
				xy_tcb_ptr[0]->local_port = 3000;
				xy_tcb_ptr[0]->inet_address_supported = inet_address_supported;
				op_prg_list_insert (tcb_list, xy_tcb_ptr[0], OPC_LISTPOS_TAIL);
					
				xy_tcb_ptr[1] = (UdpT_Tcb *) op_prg_mem_alloc (sizeof (UdpT_Tcb));
				xy_tcb_ptr[1]->app_objid = op_intrpt_source ();
				xy_tcb_ptr[1]->strm_index = strm_index;
				xy_tcb_ptr[1]->local_port = 2000;
				xy_tcb_ptr[1]->inet_address_supported = inet_address_supported;
				op_prg_list_insert (tcb_list, xy_tcb_ptr[1], OPC_LISTPOS_TAIL);
					
					
					/* Return a successful status code to the application. */
					op_ici_attr_set (ici_ptr, "status", UDPC_IND_SUCCESS);
					}
				
				/*	Obtain lan handle if this is a LAN node.	*/
				udp_lan_handle_get ();
				
				/*	Check if the node is LAN node.				*/
				if (lan_node == OPC_TRUE)
					{
					/*	This is a LAN node.	"Create" command is	*/		
					/*	issued either by client or server 		*/
					/*	(local port is 520 if "Create" command	*/
					/*	 is issued by RIP.)						*/
					if (local_port != 520)
						{
						/*	Get remote port from the ici.		*/
						op_ici_attr_get (ici_ptr, "rem_port", &rem_port);
				
				
						/*	Check if Create command is received 	*/
						/*	from server or client.					*/
						if (rem_port == -1)
							{
							/*	This command is received from the server.*/
							sess_wkstn_id = sess_svr_id;
							}
						else
							{
							sess_wkstn_id = LlmC_Unspec_Wkstn_Id;
							}
				
						/*	Register information about the	*/
						/*	new udp connection process.		*/
				printf("******local port: %d rem port:%d\n",local_port,rem_port);
						if (llm_trans_new_session (my_lanhandle, &sess_wkstn_id, rem_port, local_port) == OPC_COMPCODE_FAILURE)
							{
							/* Generate a simulation log message.	*/
							udp_llm_low_level_log_write ();
							udp_warn ("Unable to create UDP connection record in the Transport Connection Table", OPC_NIL, OPC_NIL);
							}
						
						}
					}
				
				
				//op_ici_attr_set (ici_ptr, "rem_port", 10024);
				printf("udp create******local port:%d  rem port:%d\n",local_port,rem_port);
				}
				FSM_PROFILE_SECTION_OUT (state2_enter_exec)

			/** state (CREATE) exit executives **/
			FSM_STATE_EXIT_FORCED (2, "CREATE", "xy_udp [CREATE exit execs]")


			/** state (CREATE) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "CREATE", "idle", "tr_5", "xy_udp [CREATE -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (SEND) enter executives **/
			FSM_STATE_ENTER_FORCED (3, "SEND", state3_enter_exec, "xy_udp [SEND enter execs]")
				FSM_PROFILE_SECTION_IN ("xy_udp [SEND enter execs]", state3_enter_exec)
				{
				/** Encapsulate the data packet in the UDP datagram **/
				/** and send to the lower layer. **/ 
				int 		vrf_index;
				
				
				//op_ici_format(ici_ptr,iciname);
				//printf("udp send iciname:%s.\n" ,iciname);
				//op_ici_print(ici_ptr);
				
				
				
				data_pkptr = op_pk_get (intrpt_strm);
				//data_pkptr = op_pk_get (4);
				
				
				/* The total payload size in bits */
				pk_size = (double) op_pk_total_size_get (data_pkptr);
				
				if (pk_size <= UDPC_MAX_PAYLOAD) 
					{
					UdpT_Dgram_Fields*	udp_dgram_fd_ptr;
				
					/* The payload size is less than (65535-8)*8 bits	*/
				
					/* Check whether the higher layer supports			*/
					/* the InetT_Address strucuture.					*/
					op_ici_attr_get (ici_ptr, "inet_support", &inet_address_supported);
				
					if (inet_address_supported)
						{
						/* This is a udp_command_inet ici.				*/
						op_ici_attr_get (ici_ptr, "rem_addr", &rem_addr_ptr);
				
						op_ici_attr_get (ici_ptr, "src_addr", &intf_addr_ptr);
						}
					else
						{
						/* This is a udp_command_v3 ici.				*/
						op_ici_attr_get (ici_ptr, "rem_addr", &rem_ipv4_addr);
				
						/* Convert the address into InetT_Address form.	*/
						temp_ip_addr = inet_address_from_ipv4_address_create (rem_ipv4_addr);
						rem_addr_ptr = &temp_ip_addr;
				
						/* Get the other fields in the ici.				*/
						op_ici_attr_get (ici_ptr, "src_addr", &ipv4_intf_addr);
						intf_addr = inet_address_from_ipv4_address_create_invalid_check (ipv4_intf_addr);
						intf_addr_ptr = &intf_addr;
				
						/* Since we will be using op_pk_send_forced to	*/
						/* send the packet to IP, it is OK to store the	*/
						/* address in a temporary variable.				*/
						}
				 
					op_ici_attr_get (ici_ptr, "rem_port", &rem_port);
					op_ici_attr_get (ici_ptr, "local_port", &local_port);
					op_ici_attr_get (ici_ptr, "local_minor_port", &local_minor_port);
					op_ici_attr_get (ici_ptr, "connection_class", &conn_class);
					op_ici_attr_get (ici_ptr, "Type of Service", &type_of_service);
					op_ici_attr_get_int64 (ici_ptr, "src_mac_addr", &vmac_addr);
					op_ici_attr_get_int32 (ici_ptr, "routing instance", &vrf_index);
					
					/* Using the "strm_index" to identify the outgoing IP interface	*/
					/* to allow sending multicast by setting multicast major port	*/
					op_ici_attr_get (ici_ptr, "strm_index", &intf_num);
				
					/* Create and format the UDP datagram. */
					pkptr = op_pk_create_fmt ("udp_dgram_v2");
					op_pk_nfd_set (pkptr, "data", data_pkptr);
					xy_p = op_pk_create(100);
					//op_pk_nfd_set (pkptr, "data", xy_p);
				 
					udp_dgram_fd_ptr = udp_dgram_fdstruct_create ();
					udp_dgram_fd_ptr->src_port = local_port;
					udp_dgram_fd_ptr->src_minor_port = local_minor_port;
					udp_dgram_fd_ptr->dest_port = rem_port;
					/* The length field is in byte include the header */
					udp_dgram_fd_ptr->length =  8 + (int) pk_size / 8;
				
					op_pk_nfd_set (pkptr, "fields", udp_dgram_fd_ptr, udp_dgram_fdstruct_copy, udp_dgram_fdstruct_destroy, sizeof (UdpT_Dgram_Fields)); 
				
					/* Calculate Sent Traffic statistics.  These will be	*/
					/* stored either as packets/sec or bytes/sec.			*/
					op_stat_write (sent_packet_sec_handle, 1.0);
					op_stat_write (sent_byte_sec_handle, udp_dgram_fd_ptr->length);
				
					/* Record extra data-points to enable proper			*/
					/* computation of the "sum/time" based statistics		*/
					op_stat_write (sent_packet_sec_handle, 0.0);
					op_stat_write (sent_byte_sec_handle, 0.0);
				
					/* Format the IP ici */
					op_ici_attr_set (net_ici_ptr, "dest_addr", rem_addr_ptr); 
					op_ici_attr_set (net_ici_ptr, "src_addr", intf_addr_ptr);
					op_ici_attr_set (net_ici_ptr, "connection_class", conn_class); 
					op_ici_attr_set (net_ici_ptr, "Type of Service", type_of_service);
					op_ici_attr_set_int64 (net_ici_ptr, "src_mac_addr", vmac_addr);
				
					if (-1 != vrf_index)
						op_ici_attr_set_int32 (net_ici_ptr, "vrf_index", vrf_index);
				
					/* If this is a multicast message from RIP or RIPng or HSRP, set*/
					/* the multicast ports also in the ici.					*/
					if (((rem_port == 520) || (rem_port == 521) || (rem_port == 1985) || (rem_port == 698)) &&
						(inet_address_is_multicast (*rem_addr_ptr)))
						{
						op_ici_attr_set (net_ici_ptr, "multicast_major_port", intf_num);
						op_ici_attr_set (net_ici_ptr, "multicast_minor_port", local_minor_port);
						}
					/* If this is a DHCP message destined to a client, set the outgoing
					 * interface index based on the stream index received from DHCP.  This
					 * is necessary since the destination of this packet will be a link
					 * local address, so we must know which interface to send on.
					 */
					else if (rem_port == 546)
						{
						op_ici_attr_set(net_ici_ptr, "out_intf_index", intf_num);
						}
				
					op_ici_install (net_ici_ptr);
					op_pk_send_forced (pkptr, output_strm); 
					}
				
				else
					{
					/* The payload size is larger than the maximum payload size */
					/* a UDP datagram can carry which is (65535-8) bytes.       */
					/* Drop the packet and write a log message.  			    */
					
					udp_payload_too_large_log_write (op_pk_id (data_pkptr));
					
					op_pk_destroy (data_pkptr);
					}
				}
				FSM_PROFILE_SECTION_OUT (state3_enter_exec)

			/** state (SEND) exit executives **/
			FSM_STATE_EXIT_FORCED (3, "SEND", "xy_udp [SEND exit execs]")


			/** state (SEND) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "SEND", "idle", "tr_7", "xy_udp [SEND -> idle : default / ]")
				/*---------------------------------------------------------*/



			/** state (ARRIVE) enter executives **/
			FSM_STATE_ENTER_FORCED (4, "ARRIVE", state4_enter_exec, "xy_udp [ARRIVE enter execs]")
				FSM_PROFILE_SECTION_IN ("xy_udp [ARRIVE enter execs]", state4_enter_exec)
				{
				const UdpT_Dgram_Fields*	udp_dgram_fd_ro_ptr;
				
				
				
				//op_ici_format(ici_ptr,iciname);
				//printf("udp receive iciname:%s.\n" ,iciname);
				//op_ici_print(ici_ptr);
				
				
				
				/* Get the received packet. */
				pkptr = op_pk_get (input_strm);
				
				
				
				
				/* Compute total size of packet in bits, and from that	*/
				/* Compute the byte size of this packet.				*/
				pk_size = (double) op_pk_total_size_get (pkptr);
				
				/* Calculate Receiving Traffic statistics. These will be*/
				/* stored either as packets/sec or bytes/sec.			*/
				op_stat_write (receive_packet_sec_handle, 1.0);
				op_stat_write (receive_byte_sec_handle, pk_size/8.0);
				
				/* Record extra data-points to enable proper			*/
				/* computation of the "sum/time" based statistics		*/
				op_stat_write (receive_packet_sec_handle, 0.0);
				op_stat_write (receive_byte_sec_handle, 0.0);
				
				/* UDP does not use the dest_addr info in the packet,	*/
				/* but we need to free the memory to avoid a memory leak*/
				op_ici_attr_get (ici_ptr, "dest_addr", &rem_addr_ptr);
				inet_address_destroy_dynamic (rem_addr_ptr);
				
				/* Determine the address information from the packet.	*/
				/* Note that context is switching from sender to		*/
				/* receiver, so variable names change appropriately.	*/
				op_ici_attr_get (ici_ptr, "src_addr", &rem_addr_ptr);
				
				/* Determine the type of service. It will be send to the higher layer in ICI. */
				op_ici_attr_get (ici_ptr, "Type of Service", &type_of_service);
				
				op_pk_nfd_access_read_only_ptr (pkptr, "fields", (const void **) &udp_dgram_fd_ro_ptr);
				
				
				
				rem_port = udp_dgram_fd_ro_ptr->src_port;
				local_port = udp_dgram_fd_ro_ptr->dest_port;
				
				
				printf("udp.arrival:local port:%d  rem port:%d\n",local_port,rem_port);
				//llm_trans_new_session (my_lanhandle, &sess_wkstn_id, rem_port, local_port);
				//rem_port = 10024;
				//local_port = 41196;
				
				
				if (udp_trace_active)
					{
					sprintf (msg0, "Local port: %d \t Remote Port: %d", local_port, rem_port);
					op_prg_odb_print_major ("Received UDP datagram:", msg0, OPC_NIL);
					}
				
				/* Obtain the interface address from the installed ICI.	*/
				/* A handle to this ICI has been obtained in the exit	*/
				/* execs of the "wait" state.							*/
				/* However, do not destroy this address, it is passed to*/
				/* application layer.									*/
				op_ici_attr_get (ici_ptr, "interface_received", &intf_addr_ptr);
				
				/* Obtain the interface index from the installed ICI.	*/
				/* This may be useful for some higher layers.			*/
				op_ici_attr_get (ici_ptr, "incoming_intf_index", &incoming_intf_index);
				
				/* Destroy memory allocated by the interface control	*/
				/* information associated with this interrupt.			*/
				op_ici_destroy (ici_ptr);
				
				/* Find the destination TCB from the port.				*/
				tcb_ptr = udp_tcb_from_port (local_port);
				if (tcb_ptr == OPC_NIL)
					{
					/* No match was found. Discard the datagram.		*/
				
				//printf("udp.arrival: tcb_ptr is null\n");
					
					if (udp_trace_active)
						{
						sprintf (msg0, "Datagram received for unknown port %d", local_port);
						op_prg_odb_print_major (msg0, "Discarding datagram.", OPC_NIL);
						}
				
					/* Generate a simulation log message. */
					udp_dgram_discard_log_write (local_port);
				
					op_pk_destroy (pkptr);
				
					/* Free the memory allocated to the addresses in	*/
					/* ici also.										*/
					inet_address_destroy_dynamic (rem_addr_ptr);
					inet_address_destroy_dynamic (intf_addr_ptr);
					}
				 
				else
					{
					/* A port match was found. Forward the datagram to	*/
					/* the application.									*/
					/* Check whether the application supports			*/
					/* InetT_Address structure.							*/
					if (tcb_ptr->inet_address_supported)
						{
						/* Use the udp_command_inet ici.				*/
						app_ici_ptr = app_ici_v4_ptr;
				
						op_ici_attr_set (app_ici_ptr, "rem_addr", rem_addr_ptr);
						op_ici_attr_set (app_ici_ptr, "interface received", intf_addr_ptr);	
						}
					else
						{
						/* Use the udp_command_v3 ici.					*/
						app_ici_ptr = app_ici_v3_ptr;
				
						/* Convert the address to the IpT_Address format*/
						/* before setting them in the Ici.				*/
						op_ici_attr_set (app_ici_ptr, "rem_addr", inet_ipv4_address_get (*rem_addr_ptr)); 
						/* Free the memory allocated to rem_addr_ptr	*/
						op_prg_mem_free (rem_addr_ptr);
				
						op_ici_attr_set (app_ici_ptr, "interface received", inet_ipv4_address_get (*intf_addr_ptr));	
						/* Free the memory allocated to intf_addr_ptr	*/
						op_prg_mem_free (intf_addr_ptr);
						}
				
					/* Set the remaining fields also in the Ici.		*/
					op_ici_attr_set (app_ici_ptr, "Type of Service", type_of_service); 	 
					op_ici_attr_set (app_ici_ptr, "rem_port", rem_port);
					op_ici_attr_set (app_ici_ptr, "local_port", local_port);
					op_ici_attr_set (app_ici_ptr, "incoming_intf_index", incoming_intf_index);
				
					op_ici_install (app_ici_ptr);
				
					op_pk_nfd_get (pkptr, "data", &data_pkptr);
				
				
				
				
				
				
				/*
				op_pk_fd_get(data_pkptr,0,&q);
				for(i=0;i<pk_len/8;i++){
					
					for(len=128;len >0;  len = len >> 1){
						if(q->ch[i] & len)
								printf("1");
						else
							printf("0");
					}
					printf("_");
						
				}
					printf("\n");
				*/
					
				//if( (op_pk_fd_get(data_pkptr,0,&xy_pk_ptr)==OPC_COMPCODE_FAILURE) || (xy_pk_ptr==OPC_NIL) )
				//{
				
				//printf("udp arrive none.\n");
				
				
				//}		
				
					
				//xy_pk_ptr = op_pk_create_fmt("xy_pk_fmt");
				//op_pk_nfd_set(xy_pk_ptr,"port", local_port);
				//op_pk_nfd_set(xy_pk_ptr,"pk",data_pkptr);
				//data_pkptr = xy_pk_ptr;
					
				//xy_pk_ptr = op_pk_create(0);
				//op_pk_fd_set(xy_pk_ptr,0,OPC_FIELD_TYPE_PACKET, data_pkptr,-1);
				//data_pkptr = xy_pk_ptr;
					
				pk_len = op_pk_total_size_get (data_pkptr) ;
				printf("udp arrive size: %d ..\n",pk_len);
					
				//xy_pk_ptr = op_pk_create(100);
					
				//op_pk_send_forced (data_pkptr, tcb_ptr->strm_index);
				op_pk_send_forced (data_pkptr, 4);
				
					/* Destroy the UDP header.							*/
					op_pk_destroy (pkptr);  
					}
				}
				FSM_PROFILE_SECTION_OUT (state4_enter_exec)

			/** state (ARRIVE) exit executives **/
			FSM_STATE_EXIT_FORCED (4, "ARRIVE", "xy_udp [ARRIVE exit execs]")


			/** state (ARRIVE) transition processing **/
			FSM_TRANSIT_FORCE (1, state1_enter_exec, ;, "default", "", "ARRIVE", "idle", "tr_17", "xy_udp [ARRIVE -> idle : default / ]")
				/*---------------------------------------------------------*/



			}


		FSM_EXIT (0,"xy_udp")
		}
	}




void
_op_xy_udp_diag (OP_SIM_CONTEXT_ARG_OPT)
	{
#if defined (OPD_ALLOW_ODB)
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = __LINE__+1;
#endif

	FIN_MT (_op_xy_udp_diag ())

	if (1)
		{

		/* Diagnostic Block */

		BINIT
		{
		/* We will define local variables here so that the declaration is close	*/
		/* to the place of use. Not declaring them in the TV block will also	*/
		/*  prevent compiler warnings about	unused variables. msg0 is already	*/
		/* defined in TV block and is used in other places in code too.			*/
		int				list_size				 	= 0;
		int				i							= 0;
		char			msg0 [512];
		char 			msg1 [512];
		UdpT_Tcb*		tcb_ptr;
									 
		op_prg_odb_print_major ("TCB list:", OPC_NIL);
		list_size = op_prg_list_size (tcb_list);
		for (i = 0; i < list_size; i++)
			{
			tcb_ptr = (UdpT_Tcb *) op_prg_list_access (tcb_list, i);
		
			/* Print socket information. */
			sprintf (msg0, "Port (%d) information: ", tcb_ptr->local_port);
			sprintf (msg1, "Application objid (%d), traffic through stream (%d)",
				tcb_ptr->app_objid, tcb_ptr->strm_index);
			op_prg_odb_print_major (msg0, msg1, OPC_NIL);
			}
		}

		/* End of Diagnostic Block */

		}

	FOUT
#endif /* OPD_ALLOW_ODB */
	}




void
_op_xy_udp_terminate (OP_SIM_CONTEXT_ARG_OPT)
	{

	FIN_MT (_op_xy_udp_terminate ())


	/* No Termination Block */

	Vos_Poolmem_Dealloc (op_sv_ptr);

	FOUT
	}


/* Undefine shortcuts to state variables to avoid */
/* syntax error in direct access to fields of */
/* local variable prs_ptr in _op_xy_udp_svar function. */
#undef tcb_list
#undef net_ici_ptr
#undef app_ici_v3_ptr
#undef udp_trace_active
#undef port_id_new
#undef own_mod_objid
#undef own_node_objid
#undef own_prohandle
#undef own_process_record_handle
#undef proc_model_name
#undef my_lanhandle
#undef lan_node
#undef sess_svr_id
#undef sess_wkstn_id
#undef input_strm
#undef output_strm
#undef sent_packet_sec_handle
#undef sent_byte_sec_handle
#undef receive_packet_sec_handle
#undef receive_byte_sec_handle
#undef app_ici_v4_ptr

#undef FIN_PREAMBLE_DEC
#undef FIN_PREAMBLE_CODE

#define FIN_PREAMBLE_DEC
#define FIN_PREAMBLE_CODE

VosT_Obtype
_op_xy_udp_init (int * init_block_ptr)
	{
	VosT_Obtype obtype = OPC_NIL;
	FIN_MT (_op_xy_udp_init (init_block_ptr))

	obtype = Vos_Define_Object_Prstate ("proc state vars (xy_udp)",
		sizeof (xy_udp_state));
	*init_block_ptr = 0;

	FRET (obtype)
	}

VosT_Address
_op_xy_udp_alloc (VosT_Obtype obtype, int init_block)
	{
#if !defined (VOSD_NO_FIN)
	int _op_block_origin = 0;
#endif
	xy_udp_state * ptr;
	FIN_MT (_op_xy_udp_alloc (obtype))

	ptr = (xy_udp_state *)Vos_Alloc_Object (obtype);
	if (ptr != OPC_NIL)
		{
		ptr->_op_current_block = init_block;
#if defined (OPD_ALLOW_ODB)
		ptr->_op_current_state = "xy_udp [init enter execs]";
#endif
		}
	FRET ((VosT_Address)ptr)
	}



void
_op_xy_udp_svar (void * gen_ptr, const char * var_name, void ** var_p_ptr)
	{
	xy_udp_state		*prs_ptr;

	FIN_MT (_op_xy_udp_svar (gen_ptr, var_name, var_p_ptr))

	if (var_name == OPC_NIL)
		{
		*var_p_ptr = (void *)OPC_NIL;
		FOUT
		}
	prs_ptr = (xy_udp_state *)gen_ptr;

	if (strcmp ("tcb_list" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->tcb_list);
		FOUT
		}
	if (strcmp ("net_ici_ptr" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->net_ici_ptr);
		FOUT
		}
	if (strcmp ("app_ici_v3_ptr" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->app_ici_v3_ptr);
		FOUT
		}
	if (strcmp ("udp_trace_active" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->udp_trace_active);
		FOUT
		}
	if (strcmp ("port_id_new" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->port_id_new);
		FOUT
		}
	if (strcmp ("own_mod_objid" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->own_mod_objid);
		FOUT
		}
	if (strcmp ("own_node_objid" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->own_node_objid);
		FOUT
		}
	if (strcmp ("own_prohandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->own_prohandle);
		FOUT
		}
	if (strcmp ("own_process_record_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->own_process_record_handle);
		FOUT
		}
	if (strcmp ("proc_model_name" , var_name) == 0)
		{
		*var_p_ptr = (void *) (prs_ptr->proc_model_name);
		FOUT
		}
	if (strcmp ("my_lanhandle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->my_lanhandle);
		FOUT
		}
	if (strcmp ("lan_node" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->lan_node);
		FOUT
		}
	if (strcmp ("sess_svr_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->sess_svr_id);
		FOUT
		}
	if (strcmp ("sess_wkstn_id" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->sess_wkstn_id);
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
	if (strcmp ("sent_packet_sec_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->sent_packet_sec_handle);
		FOUT
		}
	if (strcmp ("sent_byte_sec_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->sent_byte_sec_handle);
		FOUT
		}
	if (strcmp ("receive_packet_sec_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->receive_packet_sec_handle);
		FOUT
		}
	if (strcmp ("receive_byte_sec_handle" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->receive_byte_sec_handle);
		FOUT
		}
	if (strcmp ("app_ici_v4_ptr" , var_name) == 0)
		{
		*var_p_ptr = (void *) (&prs_ptr->app_ici_v4_ptr);
		FOUT
		}
	*var_p_ptr = (void *)OPC_NIL;

	FOUT
	}

