MIL_3_Tfile_Hdr_ 145A 140A modeler 9 5F86B386 5F97791D 3E DESKTOP-93M2PRA xiang 0 0 none none 0 0 none E5050CAA DFB 0 0 0 0 0 0 1bcc 1                                                                                                                                                                                                                                                                                                                                                                                          ��g�      @   D   H        �  �  �  �  �  �  �             	   begsim intrpt             ����      doc file            	nd_module      endsim intrpt             ����      failure intrpts            disabled      intrpt interval         ԲI�%��}����      priority              ����      recovery intrpts            disabled      subqueue                     count    ���   
   ����   
      list   	���   
          
      super priority             ����             Objid	\own_id;       char	\name[20];       Objid	\own_node_id;       Prohandle	\own_prohandle;          //int key=1000,port,local_port;       //Objid own_mod_objid;   //char proc_model_name[20];   //Packet* pk_ptr;   Packet* xy_pk_ptr;      typedef struct   {   	   	char ch[1000];       } xy_pk_fm;       xy_pk_fm p;   xy_pk_fm *q=&p;       int i,pk_len,len;       R#define DATA_INTRPT	(op_intrpt_type() == OPC_INTRPT_STRM && op_intrpt_strm() == 0)                                             �   �          
   st_0   
       
   A           //��ȡģ��ID   own_id= op_id_self();   	   	   //��ȡ���̵�����   *op_ima_obj_attr_get(own_id, "name", name);        //�������жϣ��ȴ��ײ��ʼ�����   *op_intrpt_schedule_self(op_sim_time(), 0);                                   /*   own_mod_objid = op_id_self ();   =op_ima_obj_attr_get (own_mod_objid, "name", proc_model_name);   (if(strcmp("p_12",proc_model_name) == 0){   	local_port = 2000;   }   (if(strcmp("p_13",proc_model_name) == 0){   	local_port = 3000;   }   */       //xy_pk_ptr = op_pk_get(0);       //op_pk_print(xy_pk_ptr);       //op_pk_send(xy_pk_ptr,2);       //op_pk_print(xy_pk_ptr);   )//op_pk_nfd_get(xy_pk_ptr, "port",&port);   )//op_pk_nfd_get(xy_pk_ptr, "pk",&pk_ptr);       /*   if(local_port == port){   8	printf("%s receive %d packet.\n",proc_model_name,port);   )	pk_len = op_pk_total_size_get (pk_ptr) ;   	op_pk_fd_get(pk_ptr,0,&q);   	for(i=0;i*8<pk_len;i++){   	   &	for(len=128;len >0;  len = len >> 1){   		if(q->ch[i] & len)   				printf("1");   		else   			printf("0");   	}   		printf("_");	   	}   	printf("\n");   	   }   */                           
                         ����             pr_state        v   �          
   st_1   
       J                  xy_pk_ptr = op_pk_get(0);       if(xy_pk_ptr != OPC_NIL){   :    printf("%s reseive packet, send to external.\n",name);       op_pk_send(xy_pk_ptr,0);       }else{   	printf("packet is null.\n");       }       J                     
    ����   
          pr_state                     �   �     �   �  �  C  �   �  |   �          
   tr_2   
       
   DATA_INTRPT   
       ����          
    ����   
          ����                       pr_transition      	            �     �   �  o   �          
   tr_9   
       ����          ����          
    ����   
          ����                       pr_transition         
                                    