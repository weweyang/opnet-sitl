MIL_3_Tfile_Hdr_ 145A 140A modeler 9 5F911837 5F97BC3E 7E DESKTOP-93M2PRA xiang 0 0 none none 0 0 none 19D901BA 2003 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                         ��g�      @   D   H      G  �  �  �  �  �  �  �  ;           	   begsim intrpt             ����      doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����              Objid	\own_id;       Objid	\own_node_id;       Ici *	\app_ici_ptr;       int	\input_strm;       int	\output_strm;       int	\send_iface;       int	\own_label;       int	\local_port;       int	\rem_port;       InetT_Address*	\rem_addr_ptr;       InetT_Address*	\intf_addr_ptr;       int	\incoming_intf_index;       int	\type_of_service;       Ici *	\ici_ptr;       Prohandle	\own_prohandle;       char	\proc_model_name[20];          int				intrpt_type;   int				intrpt_strm;   int				intrpt_code;       Packet * 			app_data;       $List*			proc_record_handle_list_ptr;   %OmsT_Pr_Handle	process_record_handle;   Objid			neighbor_mod_objid;       )//List * 				proc_record_handle_list_ptr;   )//OmsT_Pr_Handle 		process_record_handle;   '//int					proc_record_handle_list_size;       OmsT_Pr_Handle my_proc_handle;          //����ͷ�ļ�   #include <ip_addr_v4.h>   #include <ip_rte_v4.h>   #include <oms_pr.h>   #include <oms_tan.h>   #include <ip_dgram_sup.h>   #include <ip_mcast_support.h>   #include <udp_api.h>               //����״̬ת������   _#define FROM_EXTERNAL_DATA_INTRPT	(intrpt_type == OPC_INTRPT_STRM && intrpt_strm == input_strm)   ^#define TO_EXTERNAL_DATA_INTRPT		(intrpt_type == OPC_INTRPT_STRM && intrpt_strm != input_strm)                                                         �   �          
   init   
       
      	//��ȡģ��ID   	own_id= op_id_self();   	   	//��ȡ�ڵ�ID   &	own_node_id = op_topo_parent(own_id);   	   	//obtain process handle   	own_prohandle = op_pro_self();   	   	//��ȡ���̵�����   ?	op_ima_obj_attr_get(own_id, "process model", proc_model_name);       	//ע�����   ]	my_proc_handle = oms_pr_process_register(own_node_id,own_id,own_prohandle, proc_model_name);       	//���ý��̵���ز���    	oms_pr_attr_set(my_proc_handle,   2					"protocol", 		OMSC_PR_STRING, "external_app",   					OPC_NIL);   	   1	app_ici_ptr = op_ici_create("udp_command_inet");   		   !	//�������жϣ��ȴ��ײ��ʼ�����   .    op_intrpt_schedule_self(op_sim_time(), 0);   	   	       
       
       
           ����             pr_state        �   �          
   idle   
                     
   
    intrpt_type = op_intrpt_type ();   #if (intrpt_type == OPC_INTRPT_STRM)   !	intrpt_strm = op_intrpt_strm ();   else   !	intrpt_code = op_intrpt_code ();                       
           ����             pr_state        �   Z          
   send   
       J   *   //from external           ici_ptr = op_intrpt_ici ();   "app_data = op_pk_get(intrpt_strm);       3op_ici_attr_get(ici_ptr, "rem_addr",&rem_addr_ptr);       6printf("addr:%d.\n", rem_addr_ptr->address.ipv4_addr);   P//192.168.0.1 = -1062731775  192.168.0.120 = -1062731656 192.168.0.3=-1062731773       -rem_addr_ptr->address.ipv4_addr= -1062731773;           /op_ici_attr_get(ici_ptr, "rem_port",&rem_port);   3op_ici_attr_get(ici_ptr, "local_port",&local_port);       B//op_ici_attr_get (ici_ptr, "interface_received", &intf_addr_ptr);   I//op_ici_attr_get (ici_ptr, "incoming_intf_index", &incoming_intf_index);   B//op_ici_attr_get (ici_ptr, "Type of Service", &type_of_service);        switch(local_port){   	case 2000:   		op_pk_send(app_data,1);   ?		printf("external: local port:%d,send to p_12.\n",local_port);   		break;   	case 3000:   ?		printf("external: local port:%d,send to p_13.\n",local_port);   		op_pk_send(app_data,2);   		break;   		default:   K	 	printf("external:error local port:%d,destroy the packet.\n",local_port);   		op_pk_destroy(app_data);   	   		       }                       J                     
   ����   
          pr_state        �  J          
   receive   
       
      //from opnet       //�����ж϶˿�����ȡӦ������   #app_data = op_pk_get(intrpt_strm);	       7op_ici_attr_set(app_ici_ptr, "strm_index", input_strm);   :op_ici_attr_set (app_ici_ptr, "rem_addr", rem_addr_ptr);	    4op_ici_attr_set (app_ici_ptr, "rem_port", rem_port);   8op_ici_attr_set (app_ici_ptr, "local_port", local_port);       E//op_ici_attr_set (app_ici_ptr, "interface received", intf_addr_ptr);   E//op_ici_attr_set (app_ici_ptr, "Type of Service", type_of_service);    L//op_ici_attr_set (app_ici_ptr, "incoming_intf_index", incoming_intf_index);   	       //op_ici_print(app_ici_ptr);           op_ici_install(app_ici_ptr);   !op_pk_send(app_data,output_strm);                       
                     
   ����   
          pr_state        v   �          
   wait_0   
       
       //�������жϣ��ȴ��ײ��ʼ�����   *op_intrpt_schedule_self(op_sim_time(), 0);   
       
      >/* Find the stream index connect this module to lower layer	*/   G/* module - which is external_app. Obtain the process registry for  	*/   6/* the external_app process model in this node						*/   4proc_record_handle_list_ptr = op_prg_list_create ();   Homs_pr_process_discover (OPC_OBJID_INVALID, proc_record_handle_list_ptr,   +	"node objid",	OMSC_PR_OBJID,		own_node_id,   %	"protocol",		OMSC_PR_STRING,		"udp",   
	OPC_NIL);       6/* Assume there is only one udp module in the node 	*/   F/* Get the process record handle for the external_app process model	*/   lprocess_record_handle = (OmsT_Pr_Handle) op_prg_list_access (proc_record_handle_list_ptr, OPC_LISTPOS_HEAD);       0/* Obtain the module object id of the udp.				*/   \oms_pr_attr_get (process_record_handle, "module objid", OMSC_PR_OBJID, &neighbor_mod_objid);   		   6/* Determine the input and output stream indices.			*/   Voms_tan_neighbor_streams_find (own_id, neighbor_mod_objid, &input_strm, &output_strm);       L//printf("external input strm:%d output strm:%d.\n",input_strm,output_strm);       //* Deallocate the registry list pointer.					*/   6while (op_prg_list_size (proc_record_handle_list_ptr))   	{   D	op_prg_list_remove (proc_record_handle_list_ptr, OPC_LISTPOS_HEAD);   	}   .op_prg_mem_free (proc_record_handle_list_ptr);   
           ����             pr_state                     �   �     �   �  �   h          
   tr_1   
       
   FROM_EXTERNAL_DATA_INTRPT   
       ����          
    ����   
          ����                       pr_transition              �   �     �   i  �   �          
   tr_2   
       ����          ����          
    ����   
          ����                       pr_transition              A       �   �  �  5          
   tr_4   
       
   TO_EXTERNAL_DATA_INTRPT   
       ����          
    ����   
          ����                       pr_transition              �       �  2  �   �          
   tr_6   
       ����          ����          
    ����   
          ����                       pr_transition              �   �     �   �  �   �          
   tr_12   
       ����          ����          
    ����   
          ����                       pr_transition                  �     �   �  f   �          
   tr_13   
       ����          ����          
    ����   
          ����                       pr_transition                                             