namespace �̺�Ʈ_���ڻ�_����_��縶_���
{
	
	// ================ CUSTOMIZE ================
		
        const bool �ѽ�������_���麸�� = true;        // ���ϵ� �� ������ �����ϴ� �ѽ������� ���� ����
		const int �ѽ�������_����_����_���� = ����_���屺;   	// �ѽ������� ������ ������ ������ ���� 	
	
        const bool Ÿ������_�������� = true;        // ������ ����ϴ� Ÿ�� ���� ���� �߻� ����
		const int Ÿ������_����_����_���� = ����_���屺;   	// Ÿ�� ���� ������ ������ ������ ���� 			
		const int Ÿ������_��ȣ���϶��� = 50;   	// �϶��ϴ� ��ȣ�����ġ

		const int ����_ȿ��_����_���� = ����_���屺;   	// ���� �̺�Ʈ �߰� ȿ����, ���� ������ ����
		
        const bool ���ְ�_���þȿ�_�־��_�̺�Ʈ�߻� = true;        // ���ְ� ���þȿ� �־�߸� �̺�Ʈ �߻�, false�� ���ְ� �����־ ������� �ٷ� �߻�
		
	// ===========================================
	
	
	class Main
	{
		Main()
		{
		   pk::bind(112, pk::trigger112_t(callback_peerage));	
		   pk::bind(112, pk::trigger112_t(callback_peerage));
		   pk::bind(112, pk::trigger112_t(callback_peerage));
		   pk::bind(112, pk::trigger112_t(callback_peerage));
		   pk::bind(112, pk::trigger112_t(callback_peerage));
		   pk::bind(112, pk::trigger112_t(callback_peerage));		   
			
		}
		
		pk::force@ force;
		pk::person@ kunshu;
		pk::person@ gunshi;
		pk::person@ emperor; 
		pk::building@ building;
		pk::person@ senior1;
		pk::person@ senior2;
		pk::person@ senior3;
		pk::person@ senior4;
		pk::person@ senior5;
		pk::person@ senior6;
		
				
		pk::person@ maverick1;

		pk::force@ enemy_force;
		pk::force@ enemy_force_1;
		pk::person@ enemy_kunshu;
		pk::building@ enemy_building;

		pk::force@ pro_han_force_1;
		pk::person@ pro_han_kunshu;
		pk::building@ pro_han_building;



        //----------------------------------------------------------------------------------
        //  ���� �̺�Ʈ
        //----------------------------------------------------------------------------------


// ���� �̺�Ʈ

		void callback_peerage(pk::force@ force)
		{
                                  @emperor = pk::get_person(pk::get_scenario().emperor);

		      if (!force.is_player()) return;

            int city_count = 0;
       if (force.title == ����_����)	city_count = 2;		    // ����_����
       if (force.title == ����_���ڻ�)	city_count = 4;		// ����_���ڻ�
       if (force.title == ����_�ָ�)	city_count = 6;	        // ����_�ָ�
       if (force.title == ����_�츲�߶���)	city_count = 8;	    // ����_�츲�߶���
       if (force.title == ����_�����߶���)	city_count = 12;    // ����_�����߶���
       if (force.title == ����_���屺)	city_count = 14;	// ����_���屺   

         for (int p = ����_���屺; p < ����_��; p++)
         {			
			if (force.is_player() and force.title == p and pk::get_city_count(force) >= city_count)
			{
				@force = force;
				@kunshu = pk::get_person(force.kunshu);
				@gunshi = pk::get_person(force.gunshi);
				@building = pk::get_building(kunshu.service);

		   // ���ְ� ���ÿ� ���� ����
		if (���ְ�_���þȿ�_�־��_�̺�Ʈ�߻�)
		{		   
            int building_id = building.get_id();
            if (pk::is_absent(kunshu)) return;
            if (pk::is_unitize(kunshu)) return;
            if (kunshu.get_force_id() != building.get_force_id()) return;		
            if (kunshu.location != kunshu.service) return;		
            if (kunshu.mibun == �ź�_����) return;
            if (�ǹ�_�������� <= building_id and building_id < �ǹ�_�ױ���) return;	        
	    }
		
			string current_title_name = pk::decode(pk::get_name(pk::get_title(p)));
			string promotion_title_name = pk::decode(pk::get_name(pk::get_title(p - 1)));	

            int prev_title = force.title;			


		   // ���� �ֵ� ���� �׷��

                                 pk::list<pk::person@> senior_list;
                                 senior_list.clear();								 
                                 auto member_list = pk::list_to_array(pk::get_person_list(force, pk::mibun_flags(�ź�_����,�ź�_�¼�,�ź�_�Ϲ�) ) );

                                 if (0 < member_list.length)
                                  {
           	                      for (int j = 0; j < member_list.length; j++)
           	                      {
	                       pk::person@ member = member_list[j];	
                                     if (pk::is_alive(member) and member.get_id() != kunshu.get_id()) 	
                                     senior_list.add(member);

	                       }

                                 }


                       if (0 == senior_list.count)
                       {

                                   @senior1 = pk::get_person(����_����);
                                   @senior2 = pk::get_person(����_����);	
                                   @senior3 = pk::get_person(����_����);	
                                   @senior4 = pk::get_person(����_����);	
                                   @senior5 = pk::get_person(����_��ȣ);	
                                   @senior6 = pk::get_person(����_����);
	                   }

                       else if (0 < senior_list.count)
                       {
	                      senior_list.sort(function(a, b)
	                      {				
	                      return float(( a.loyalty * 30 ) / (1 +  (4 * a.rank))) > float(( b.loyalty * 30 ) / (1 + (4 * b.rank)));
				
	                      });

                                   @senior1 = 0 < senior_list.count ? senior_list[0] : pk::get_person(����_����);
                                   @senior2 = 1 < senior_list.count ? senior_list[1] : pk::get_person(����_����);	
                                   @senior3 = 2 < senior_list.count ? senior_list[2] : pk::get_person(����_����);	
                                   @senior4 = 3 < senior_list.count ? senior_list[3] : pk::get_person(����_����);	
                                   @senior5 = 4 < senior_list.count ? senior_list[4] : pk::get_person(����_��ȣ);	
                                   @senior6 = 5 < senior_list.count ? senior_list[5] : pk::get_person(����_����);
	                   }								   



             // ���� �� �ѿս� ��ȣ�����ڵ�
                                 pk::list<pk::person@> loyalist_list;
                                 loyalist_list.clear();									 
                                 auto hansilman_list = pk::list_to_array(pk::get_person_list(force, pk::mibun_flags(�ź�_����,�ź�_�¼�,�ź�_�Ϲ�) ) );

                                 if (0 < hansilman_list.length)
                                  {
           	                      for (int j = 0; j < hansilman_list.length; j++)
           	                      {
	                       pk::person@ loyalist = hansilman_list[j];	
                                     if (pk::is_alive(loyalist) and loyalist.get_id() != kunshu.get_id() and loyalist.kanshitsu == �ѽ�_�߽�
									 and loyalist.get_id() != senior1.get_id() and loyalist.get_id() != senior2.get_id() and loyalist.get_id() != senior3.get_id()
									 and loyalist.get_id() != senior4.get_id() and loyalist.get_id() != senior5.get_id() and loyalist.get_id() != senior6.get_id()) 	
           	                        {
			// ���ָ� ģ���ϰų� �κ�, ������, �θ��ڽ� ������ ��� ����
			if (func_relation_check(loyalist, kunshu) == 0 and func_mutual_relation_check(loyalist, kunshu) == 0)
           	                        {
                                       loyalist_list.add(loyalist);

	                              }
	                        }
	                       }

                                 }

                       if (0 < loyalist_list.count)
                       {	                      
						  loyalist_list.sort(function(a, b)
	                      {	
	                      return float(( a.loyalty * 30 ) / (1 +  (4 * a.rank))) < float(( b.loyalty * 30 ) / (1 + (4 * b.rank)));
										  
	                      });

                                   @maverick1 = loyalist_list[0];

	                   }
				
		   // ���� �ҽĿ� �а��� Ÿ�� ����

		    pk::list<pk::force@> enemy_force_list;
            enemy_force_list.clear();			
			pk::list<pk::force@> force_list = pk::get_force_list();
			for (int i = 0; i < force_list.count; i++)
			{
	        pk::force@ enemy_force = force_list[i];
            if (enemy_force.get_id() != force.get_id()) 	
            enemy_force_list.add(enemy_force);
			}

            if (enemy_force_list.count > 0)
		    {
	           enemy_force_list.sort(function(a, b)
	           {				
	            return pk::get_city_count(a) > pk::get_city_count(b);
	           });
	                    
              @ enemy_force_1 = enemy_force_list[0];
              @ enemy_kunshu = pk::get_person(pk::get_kunshu_id(enemy_force_1));	
		      @ enemy_building = pk::get_building(enemy_kunshu.service);
		    }




		   // �̺�Ʈ ���� 
		   // Ȳ���� ������		   
			if (emperor is null)
			{		   
		      pk::scene(pk::scene_t(scene_����_õ�ھ���));
			  
               if (0 < loyalist_list.count and �ѽ�������_���麸�� and force.title <= �ѽ�������_����_����_����)
               {	
		   	    pk::scene(pk::scene_t(scene_�ѽ�������_����));
			   }

			   if (force.title == prev_title - 1 and Ÿ������_�������� and force.title <= Ÿ������_����_����_����)
			   {							  
				pk::scene(pk::scene_t(scene_����_Ÿ�����ֹ���));					
			   }				   
			}
			
		   // Ȳ���� ���� ����	   
            else if (emperor !is null)
			{	
			  pk::scene(pk::scene_t(scene_����_õ�ڰ��Ӹ�));				   

               if (0 < loyalist_list.count and �ѽ�������_���麸�� and force.title <= �ѽ�������_����_����_����)
               {	
		   	    pk::scene(pk::scene_t(scene_�ѽ�������_����));
			   }

			   if (force.title == prev_title - 1 and Ÿ������_�������� and force.title <= Ÿ������_����_����_����)
			   {							  
				pk::scene(pk::scene_t(scene_����_Ÿ�����ֹ���));					
			   }			   
			}


            pk::scene(pk::scene_t(scene_ȭ��_����));	
			
			
			if (force.title == prev_title - 1)		
			{			
		   // �����丮 �α� ���
		      pk::history_log(pk::get_current_turn_force_id(), force.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� \x1b[1x{}\x1b[0x�� �ڸ��� ������" , pk::decode(pk::get_name(kunshu)), promotion_title_name ) ) );
			}

			}

		  } // for

		} // callback_peerage
		


		void scene_����_õ�ھ���()
		{
				@force = pk::get_force(kunshu.get_force_id());
			pk::person@ subject1 = pk::is_alive(senior1) ? senior1 : pk::get_person(����_����);
			pk::person@ subject2 = pk::is_alive(senior2) ? senior2 : pk::get_person(����_����);
			pk::person@ subject3 = pk::is_alive(senior3) ? senior3 : pk::get_person(����_����);

			pk::person@ subject4 = pk::is_alive(senior4) ? senior4 : pk::get_person(����_����);
			pk::person@ subject5 = pk::is_alive(senior5) ? senior5 : pk::get_person(����_��ȣ);
			pk::person@ subject6 = pk::is_alive(senior6) ? senior6 : pk::get_person(����_����);

			string kunshuname = pk::decode(pk::get_name(kunshu));
			string subject1name = pk::decode(pk::get_name(subject1));

	
                 pk::list<pk::person@> target_list;
                 target_list.clear();				 
		         pk::person@ target_member;	
				 array<string> person_name_list(0);

				 if (pk::is_alive(subject1))
				 target_list.add(subject1);	

				 if (pk::is_alive(subject2))
				 target_list.add(subject2);	
			 
				 if (pk::is_alive(subject3))
				 target_list.add(subject3);
			 
				 if (pk::is_alive(subject4))
				 target_list.add(subject4);	

				 if (pk::is_alive(subject5))
				 target_list.add(subject5);	
			 
				 if (pk::is_alive(subject6))
				 target_list.add(subject6);			 

                if (target_list.count > 0)
		     	{
           	       for (int j = 0; j < target_list.count; j++)
           	       {
					@target_member = target_list[j];  
				  
				  person_name_list.insertLast(pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(target_member))));
		 					   
		           }
				   		        				
		        }

				  string person_names = join(person_name_list, ", "); 	
					
 
			pk::move_screen(building.get_pos());
            pk::play_bgm(3);
			pk::message_box(pk::encode(pk::format("��� ��, \x1b[1x{}\x1b[0x���� \x1b[2x{}\x1b[0x���� ã�ƿԴ�.", kunshuname, person_names)));
			pk::message_box(pk::encode("�ְ�, ���� �帱 ������ �ֽ��ϴ�..."), subject1);

			pk::council(kunshu, subject1, subject2, subject3, subject4, subject5, subject6, pk::council_t(council_����_õ�ھ���_�ߴ�));

            int prev_title = force.title;	
			string promotion_title_name = pk::decode(pk::get_name(pk::get_title(prev_title - 1)));			

			pk::background(30);
			pk::fade(255);
			
            pk::play_se(6);

			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� \x1b[1x{}\x1b[0x�� �ڸ��� �ö���.", kunshuname, promotion_title_name)));
			
		   // ���� ����	
		   
		      force.title = prev_title - 1;
		      force.update();
			  
            if (force.title <= ����_ȿ��_����_����)
            {				
			pk::play_se(10);
			pk::cutin(21);
			pk::play_se(35); 			
	        }			  

            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x ������ ���ϵ帳�ϴ�.", promotion_title_name)), subject1);
			pk::message_box(pk::encode("�����е��� ��� �Բ� ������ �����̴�."), kunshu);
            pk::message_box(pk::encode(pk::format("�츮 ���� ���µ� �����ϰ� �ְ� \x1b[1x{}\x1b[0x �����̶�� ���籺��. ���ϵ帳�ϴ�.", promotion_title_name)), subject4);			
			pk::message_box(pk::encode("���ϵ帳�ϴ�, �ֱ�!"), subject5);			
			pk::message_box(pk::encode("��ε� ����. �����е��� �츮 ���� ���� �� ���������ֱ� �ٶ���."), kunshu);
			pk::message_box(pk::encode("�������ʽÿ�. ���� �ְ��� ���ڽ��ϴ�."), subject6);		
			pk::message_box(pk::encode("�츮 ���� ������ ���� ����ϰڽ��ϴ�!"), subject3);			
            pk::message_box(pk::encode(pk::format("�ŵ� \x1b[1x{}\x1b[0x ���� ���ϵ帳�ϴ�.", promotion_title_name)), subject2);				

			pk::background(38);

                                           string current_year = pk::get_year();	
                                           string current_month = pk::get_month();
                                           string current_day = pk::get_day();			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x��.\n\x1b[1x{}\x1b[0x(��)�� \x1b[1x{}\x1b[0x�� ������ ��Ī�߽��ϴ�.", current_year, current_month, current_day, kunshuname, promotion_title_name )));
					
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		
		} // void scene_����_õ�ھ���()



		void council_����_õ�ھ���_�ߴ�()
		{

			pk::person@ subject1 = pk::is_alive(senior1) ? senior1 : pk::get_person(����_����);
			pk::person@ subject2 = pk::is_alive(senior2) ? senior2 : pk::get_person(����_����);
			pk::person@ subject3 = pk::is_alive(senior3) ? senior3 : pk::get_person(����_����);

			pk::person@ subject4 = pk::is_alive(senior4) ? senior4 : pk::get_person(����_����);
			pk::person@ subject5 = pk::is_alive(senior5) ? senior5 : pk::get_person(����_��ȣ);
			pk::person@ subject6 = pk::is_alive(senior6) ? senior6 : pk::get_person(����_����);

			string kunshuname = pk::decode(pk::get_name(kunshu));
			string subject1name = pk::decode(pk::get_name(subject1));
			
            int prev_title = force.title;	
			string promotion_title_name = pk::decode(pk::get_name(pk::get_title(prev_title - 1)));			
		
			
            pk::message_box(pk::encode(pk::format("�ְ�, �ֱ� �츮 ���� �ְ��� Ȱ���� �̹� ������ �˷��� �� ������ ��ġ�� �ֽ��ϴ�. �̿� ���� ���ŵ�� �Źε��� �ְ����� \x1b[1x{}\x1b[0x�� ������ �ֽñ� �ٶ�� �ֽ��ϴ�.", promotion_title_name)), subject1);			
            pk::message_box(pk::encode(pk::format("�׷����ϴ�. ���� \x1b[1x{}\x1b[0x�� ������ �� �Դϴ�.", promotion_title_name)), subject2);			
			pk::message_box(pk::encode("�츮 ���� ������ Ŀ�� ��ŭ, ���ο� ������ �����ž� �մϴ�."), subject3);			
            pk::message_box(pk::encode(pk::format("�˰ڴ�. �Ⲩ�� \x1b[1x{}\x1b[0x�� ������ �����ڴ�. �̷��� ������ �Źε鿡�� �����ϵ��� �϶�.", promotion_title_name)), kunshu);				



		} // council_����_õ�ھ���_�ߴ�





		void scene_����_õ�ڰ��Ӹ�()
		{
				@force = pk::get_force(kunshu.get_force_id());
			pk::person@ subject1 = pk::is_alive(senior1) ? senior1 : pk::get_person(����_����);
			pk::person@ subject2 = pk::is_alive(senior2) ? senior2 : pk::get_person(����_����);
			pk::person@ subject3 = pk::is_alive(senior3) ? senior3 : pk::get_person(����_����);

			pk::person@ subject4 = pk::is_alive(senior4) ? senior4 : pk::get_person(����_����);
			pk::person@ subject5 = pk::is_alive(senior5) ? senior5 : pk::get_person(����_��ȣ);
			pk::person@ subject6 = pk::is_alive(senior6) ? senior6 : pk::get_person(����_����);

			string kunshuname = pk::decode(pk::get_name(kunshu));
			string subject1name = pk::decode(pk::get_name(subject1));
			string emperorname = pk::decode(pk::get_name(emperor));
						
			pk::move_screen(building.get_pos());
            pk::play_bgm(3);
			pk::message_box(pk::encode(pk::format("��� ��, \x1b[1x{}\x1b[0x���� Ȳ���� ���ڰ� ã�ƿԴ�.", kunshuname)));
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x��, \x1b[1x{}\x1b[0x�����κ��� Ģ�簡 �Խ��ϴ�. ������ �����Ѵٴ� �ҽ��Դϴ�.", kunshuname, emperorname)), pk::get_person(����_����));

			pk::person@ aide = pk::is_alive(gunshi) ? gunshi : subject1;

			pk::diplomacy(kunshu, aide, pk::get_person(����_����), pk::diplomacy_t(diplomacy_����_õ����_���ڿ�_����));



			pk::background(30);
			pk::fade(255);	
			
            pk::play_se(6);

            int prev_title = force.title;	
			string promotion_title_name = pk::decode(pk::get_name(pk::get_title(prev_title - 1)));			
		
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� \x1b[1x{}\x1b[0x�� �ڸ��� �ö���.", kunshuname, promotion_title_name)));
				
		   // ���� ����
		   
		      force.title = prev_title - 1;
		      force.update();
			  
            if (force.title <= ����_ȿ��_����_����)
            {	                      
			pk::play_se(10);
			pk::cutin(21);
			pk::play_se(35); 			
	        }			  

            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x ������ ���ϵ帳�ϴ�.", promotion_title_name)), subject1);
			pk::message_box(pk::encode("�����е��� ��� �Բ� ������ �����̴�."), kunshu);
            pk::message_box(pk::encode(pk::format("�츮 ���� ���µ� �����ϰ� �ְ� \x1b[1x{}\x1b[0x �����̶�� ���籺��. ���ϵ帳�ϴ�.", promotion_title_name)), subject4);			
			pk::message_box(pk::encode("���ϵ帳�ϴ�, �ֱ�!"), subject5);			
			pk::message_box(pk::encode("��ε� ����. �����е��� �츮 ���� ���� �� ���������ֱ� �ٶ���."), kunshu);
			pk::message_box(pk::encode("�������ʽÿ�. ���� �ְ��� ���ڽ��ϴ�."), subject6);		
			pk::message_box(pk::encode("�츮 ���� ������ ���� ����ϰڽ��ϴ�!"), subject3);			
            pk::message_box(pk::encode(pk::format("�ŵ� \x1b[1x{}\x1b[0x ���� ���ϵ帳�ϴ�.", promotion_title_name)), subject2);				

			pk::background(34);
			
                                           string current_year = pk::get_year();	
                                           string current_month = pk::get_month();
                                           string current_day = pk::get_day();			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x��.\n\x1b[1x{}\x1b[0x(��)�� \x1b[1x{}\x1b[0x�κ��� \x1b[1x{}\x1b[0x�� ������ �޾ҽ��ϴ�.", current_year, current_month, current_day, kunshuname, emperorname, promotion_title_name )));
					
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		
		} // void scene_����_õ�ڰ��Ӹ�()



		void diplomacy_����_õ����_���ڿ�_����()
		{

			string kunshuname = pk::decode(pk::get_name(kunshu));
            int prev_title = force.title;	
			string promotion_title_name = pk::decode(pk::get_name(pk::get_title(prev_title - 1)));			
   
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x, �״븦 \x1b[1x{}\x1b[0x�� �Ӹ��Ѵ�. �����ε� \x1b[1x��\x1b[0x���� ���� �����϶�.", kunshuname, promotion_title_name )), pk::get_person(����_����));
			pk::message_box(pk::encode("�����մϴ�. ��뿡 ��߳��� �ʵ��� �� ������ �ϰڽ��ϴ�."), kunshu);


		} // diplomacy_����_õ����_���ڿ�_����()



		void scene_�ѽ�������_����()
		{
				@force = pk::get_force(kunshu.get_force_id());

			string kunshuname = pk::decode(pk::get_name(kunshu));
	
			string current_title = pk::decode(pk::get_name(pk::get_title(force.title)));		
   	
			
               if (kunshu.kanshitsu == �ѽ�_�߽�)
			   {
			   pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x ���� �ҽĿ� �ѽ��� �߽��ϴ� �Ϻ� ���ŵ��� ������...", kunshuname, current_title)));	
               pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x���� �и� �ѽ��� ���Ϸμ� �漺�� �� �Ͻ� ���̴�.", kunshuname )), maverick1);		   
				  
			   }

               else if (kunshu.kanshitsu != �ѽ�_�߽�)
			   {
			   pk::message_box(pk::encode(pk::format("������, \x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x ���� �ҽĿ� �ѽ��� �߽��ϴ� �Ϻ� ���ŵ��� ������...", kunshuname, current_title)));	
               pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x���� \x1b[1x{}\x1b[0x�� �Ӹ�ǽ� ���� ��� ��������... ����� ���� ������ ���� ���� ��ɵ� �� ũ�� �þ�� ���� �ڿ����� ��ġ��...�ѽ��� ���Ϸμ��� �ڰ��� ���� �����ž� �����ε�...���� ������ ������ ������...", kunshuname, current_title )), maverick1);
			   }
			   
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		
		} // void scene_�ѽ�������_����()






        //----------------------------------------------------------------------------------
        //  ���� Ÿ�� ����
        //----------------------------------------------------------------------------------



		void scene_����_Ÿ�����ֹ���()
		{
			string kunshuname = pk::decode(pk::get_name(kunshu));
		   // ���� �ҽĿ� �а��� Ÿ�� ���� ���
            if (pk::is_alive(enemy_kunshu) and pk::is_alive(enemy_force_1)) 	
		    {
             if (enemy_kunshu.get_id() == kunshu.get_id()) 
                 return;
				
			pk::move_screen(enemy_building.get_pos());
            pk::play_bgm(20);
			
			string current_title = pk::decode(pk::get_name(pk::get_title(force.title)));			
			pk::message_box(pk::encode(pk::format("����, \x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x ���� �ҽ��� �ٸ� ���ֵ鿡�� ������ ���� Ű��� ��Ⱑ �Ǿ���.", kunshuname, current_title)));
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x�� �߽��� ���� �� Ŀ���ڱ���... ����ؼ� �ȵ� ���̴�." , kunshuname)), enemy_kunshu);

		    pk::force@ enemy_force_group_1;		
		    pk::person@ enemy_group_kunshu;			
			pk::building@ enemy_group_kunshu_building;		

		    array<string> enemy_person_name_list(0);
		    pk::list<pk::force@> enemy_force_group_list;
            enemy_force_group_list.clear();			
			pk::list<pk::force@> force_list = pk::get_force_list();
			for (int i = 0; i < force_list.count; i++)
			{
	        pk::force@ enemy_force_group = force_list[i];
            if (enemy_force_group.get_id() != force.get_id())
			   {			
               pk::add_relations(enemy_force_group, force.get_id(), -Ÿ������_��ȣ���϶��� );				
               enemy_force_group_list.add(enemy_force_group);
			   }		   
			}

            if (enemy_force_group_list.count > 0)
		    {
	           enemy_force_group_list.sort(function(a, b)
	           {				
	            return pk::get_city_count(a) > pk::get_city_count(b);
	           });
	                    
              @ enemy_force_group_1 = enemy_force_group_list[0];
              @ enemy_group_kunshu = pk::get_person(pk::get_kunshu_id(enemy_force_group_1));	
		      @ enemy_group_kunshu_building = pk::get_building(enemy_group_kunshu.service);
			  
           	 for (int k = 0; k < enemy_force_group_list.count; k++)
           	 {		 
	         pk::force@ target_group = enemy_force_group_list[k];
             pk::person@ target_member = pk::get_person(pk::get_kunshu_id(target_group));
			 enemy_person_name_list.insertLast(pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(target_member))));	 					   
		     }			  
		    }

			string enemy_group_kunshuname = pk::decode(pk::get_name(enemy_group_kunshu)); 

            string person_names = join(enemy_person_name_list, ", "); 	

            pk::message_box(pk::encode(pk::format("\x1b[2x{}\x1b[0x - \x1b[2x{}\x1b[0x���� ����(��)�� \x1b[1x{}\x1b[0x�� ���� ��ȣ���� ũ�� �϶��ߴ�.", person_names, enemy_force_group_list.count, kunshuname)));
 
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);	
		    }
	
		} // scene_����_Ÿ�����ֹ���()





        //----------------------------------------------------------------------------------
        //  ��Ÿ
        //----------------------------------------------------------------------------------




// ȭ�� ����
		void scene_ȭ��_����()
		{
			pk::move_screen(building.get_pos());		
		} // scene_ȭ��_����()
	
	

// ģ���ִ� ���� ����

	 int func_relation_check(pk::person@ person, pk::person@ suspect)
		{
		 int relationcheck = 0;
		 pk::list<pk::person@> friend_list;
         friend_list.clear();			
         if (pk::is_like(person, suspect.get_id()) or pk::is_fuufu(person, suspect.get_id()) 
			 or pk::is_gikyoudai(person, suspect.get_id()) or pk::is_oyako(person, suspect.get_id())
		     or person.ketsuen == suspect.ketsuen or pk::get_aishou_distance(person, suspect.get_id()) <= 10 )			
			{
				relationcheck++;
			}
			return relationcheck;
		}		
		

// ������ ģ���ִ� ������ �ִ��� ����
	 int func_mutual_relation_check(pk::person@ person, pk::person@ suspect)
	 {
		 int relationcheck = 0;
		 pk::list<pk::person@> friend_list;
         friend_list.clear();		 
	   for (int i = 0; i < ����_��; i++)
	   {
		  pk::person@ friend = pk::get_person(i);
		  if (pk::is_alive(friend) and friend.get_id() != person.get_id() and friend.get_id() != suspect.get_id())
		  {
             if (pk::is_like(person, friend.get_id()) or pk::is_fuufu(person, friend.get_id()) 
			 or pk::is_gikyoudai(person, friend.get_id()) or pk::is_oyako(person, friend.get_id())
		     or person.ketsuen == friend.ketsuen)	

             friend_list.add(friend);			 

             if (friend_list.count > 0)
		     {
		       for (int f = 0; f < friend_list.count; f++)
		       {
			   pk::person@ friend_group_member = pk::get_person(f); 

               if (pk::is_like(friend_group_member, suspect.get_id()) or pk::is_fuufu(friend_group_member, suspect.get_id()) 
			   or pk::is_gikyoudai(friend_group_member, suspect.get_id()) or pk::is_oyako(friend_group_member, suspect.get_id())
		       or friend_group_member.ketsuen == suspect.ketsuen)

			   relationcheck++;		
			   
			   }				   
			 }	
			 
		  }	 						  
	  }
				 
			return relationcheck;
   }	


		
	} // class Main

	Main main;
}