namespace �̺�Ʈ_Ȳ��_��_��_���
{
	
	// ================ CUSTOMIZE ================

		const int duke_city_count = 16;   	// �� �̺�Ʈ ���� 16�� ����		
		const int king_city_count = 20;   	// �� �̺�Ʈ ���� 20�� ����
		const int emperor_city_count = 24;   	// Ȳ�� �̺�Ʈ ���� 24�� ����

        const bool Ȳ������_�ŷ°��� = true;        // true=����, false=������, Ȳ�����ڽ� �ŷ� ���� ����
		const int ����_�ŷ�_���ҷ� = 10;   	// Ȳ�����ڽ� �ŷ� ���ҷ�		

        const bool �����߽ð���_�漺���� = true;        // ��ȣ�� ���� �ƴҶ� ����� �����߽ð��ŵ��� �漺 ���� ����
		const int ��ȣ����_�漺���� = 30;   	// ��ȣ�� ���� �ƴҶ� ����� ���� �߽� ���ŵ��� �漺 ���ҷ�		
		const int ��ȣ����_�漺����_���� = 3;   	// ��ȣ�� ���� �ƴϱ� ������, ������ ���� �����ϴ� ���� �߽� ���ŵ��� �漺 ���ҷ�
		const int ����_�漺���� = 60;   	// Ȳ�� ������ ��ݸ��� ���� �߽� ���ŵ��� �漺 ���ҷ�

        const bool �����߽ð���_�漺���� = true;        // ��ȣ�� ������ ���� �Ǿ� ����� �����߽ð��ŵ��� �漺 ���� ����
		const int ��ȣ����_�漺���� = 10;   	// ��ȣ�� ������ �����ɶ� ���� �߽� ���ŵ��� �漺 ������
		
        const bool Ÿ������_�������� = true;        // Ȳ�� ������ �г��ϴ� Ÿ�� ���� ���� �߻� ����
		const int Ÿ������_��ȣ���϶��� = 50;   	// Ȳ�� ������ �г��Ͽ� �϶��ϴ� ��ȣ�����ġ

        const bool Ȳ��_ó�� = true;        // Ȳ�� ó���� ���� ��� ����
		const int ������ó��_�漺���� = 100;   	// Ȳ�� ó���� �г��Ͽ� �϶��ϴ� �漺��
		const int Ÿ������_��ȣ���϶���_Ȳ��ó�� = 100;   	// Ȳ�� ó���� �г��Ͽ� �϶��ϴ� ��ȣ�����ġ	

        const bool Ȳ��ó��_�ŷ°��� = true;        // Ȳ��ó���� �ŷ� ���� ����
		const int ó��_�ŷ�_���ҷ� = 20;   	// Ȳ��ó���� �ŷ� ���ҷ�

        const bool ����������_�÷��̾�_���� = true;        // Ȳ�� ������ ���� �ѽ������ڵ��� �÷��̾ �����ϱ� ����

        const bool ���ְ�_���þȿ�_�־��_�̺�Ʈ�߻� = true;        // ���ְ� ���þȿ� �־�߸� �̺�Ʈ �߻�, false�� ���ְ� �����־ ������� �ٷ� �߻�

		const int ����_���� = 0;   	// 0: ���ְ� �ѽ��߽� + Ȳ���� ģ�����������,  1: ���ְ� �ѽ��߽��̱⸸ �ϸ� ��
		
	// ===========================================
	
	
	class Main
	{
		Main()
		{
		   pk::bind(112, pk::trigger112_t(callback_duke));														
		   pk::bind(112, pk::trigger112_t(callback_king));			
		   pk::bind(112, pk::trigger112_t(callback_emperor));			
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
		
		pk::person@ pro_dethrone1;
		pk::person@ pro_dethrone2;
		pk::person@ pro_dethrone3;
		pk::person@ pro_dethrone4;
		pk::person@ pro_dethrone5;
		pk::person@ pro_dethrone6;
				
		pk::person@ maverick1;

		pk::force@ enemy_force;
		pk::force@ enemy_force_1;
		pk::person@ enemy_kunshu;
		pk::building@ enemy_building;

		pk::force@ pro_han_force_1;
		pk::person@ pro_han_kunshu;
		pk::building@ pro_han_building;



        //----------------------------------------------------------------------------------
        //  �� ���� �̺�Ʈ
        //----------------------------------------------------------------------------------


// �� ���� �̺�Ʈ

		void callback_duke(pk::force@ force)
		{
                                  @emperor = pk::get_person(pk::get_scenario().emperor);

		      if (!force.is_player()) return;
		      if (force.title != ����_��縶) return;
		      if (pk::get_city_count(force) < duke_city_count ) return;

			
			if (force.is_player() and force.title == ����_��縶 and pk::get_city_count(force) >= duke_city_count)
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



           // Ȳ�� ���� ���� ���� ����
                                 pk::list<pk::person@> pro_dethrone_list;
                                 pro_dethrone_list.clear();								 
                                 auto radical_list = pk::list_to_array(pk::get_person_list(force, pk::mibun_flags(�ź�_����,�ź�_�¼�,�ź�_�Ϲ�) ) );

                                 if (0 < radical_list.length)
                                  {
           	                      for (int k = 0; k < radical_list.length; k++)
           	                      {
	                       pk::person@ radical = radical_list[k];	
                                     if (pk::is_alive(radical) and radical.get_id() != kunshu.get_id() and radical.kanshitsu == �ѽ�_����) 	
                                     pro_dethrone_list.add(radical);

	                       }

                                 }


                       if (0 == pro_dethrone_list.count)
                       {

                                   @pro_dethrone1 = pk::get_person(����_����);
                                   @pro_dethrone2 = pk::get_person(����_����);	
                                   @pro_dethrone3 = pk::get_person(����_����);	
                                   @pro_dethrone4 = pk::get_person(����_����);	
                                   @pro_dethrone5 = pk::get_person(����_��ȣ);	
                                   @pro_dethrone6 = pk::get_person(����_����);
								   
	                   }

                       else if (0 < pro_dethrone_list.count)
                       {
	                      pro_dethrone_list.sort(function(a, b)
	                      {				
	                      return float(( a.loyalty * 30 ) / (1 +  (4 * a.rank))) > float(( b.loyalty * 30 ) / (1 + (4 * b.rank)));
 
	                      });

                                   @pro_dethrone1 = 0 < pro_dethrone_list.count ? pro_dethrone_list[0] : pk::get_person(����_����);
                                   @pro_dethrone2 = 1 < pro_dethrone_list.count ? pro_dethrone_list[1] : pk::get_person(����_����);	
                                   @pro_dethrone3 = 2 < pro_dethrone_list.count ? pro_dethrone_list[2] : pk::get_person(����_����);	
                                   @pro_dethrone4 = 3 < pro_dethrone_list.count ? pro_dethrone_list[3] : pk::get_person(����_����);	
                                   @pro_dethrone5 = 4 < pro_dethrone_list.count ? pro_dethrone_list[4] : pk::get_person(����_��ȣ);	
                                   @pro_dethrone6 = 5 < pro_dethrone_list.count ? pro_dethrone_list[5] : pk::get_person(����_����);

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


		   // ���� �ҽĿ� �а��� �ѽ� �߽� ����

		    pk::list<pk::force@> pro_han_force_list;
            pro_han_force_list.clear();			
			for (int i = 0; i < force_list.count; i++)
			{
	        pk::force@ pro_han_force = force_list[i];
	        pk::person@ pro_han_kunshu_group = pk::get_person(pk::get_kunshu_id(pro_han_force));
            if (pro_han_force.get_id() != force.get_id() and pro_han_kunshu_group.kanshitsu == �ѽ�_�߽�) 	
            pro_han_force_list.add(pro_han_force);
			}

            if (pro_han_force_list.count > 0)
		    {
	           pro_han_force_list.sort(function(a, b)
	           {				
	            return pk::get_city_count(a) > pk::get_city_count(b);
	           });
	                    
              @ pro_han_force_1 = pro_han_force_list[0];
              @ pro_han_kunshu = pk::get_person(pk::get_kunshu_id(pro_han_force_1));	
		      @ pro_han_building = pk::get_building(pro_han_kunshu.service);
		    }

		   // �̺�Ʈ ���� 
		   // Ȳ���� ������		   
			if (emperor is null)
			{		   
		      pk::scene(pk::scene_t(scene_������_õ�ھ���));
			  
               if (0 < loyalist_list.count)
               {	
		   	    pk::scene(pk::scene_t(scene_�ѽ�������_����_��));
			   }

			   if (force.title == ����_�� and Ÿ������_��������)
			   {							  
				pk::scene(pk::scene_t(scene_�հ�����_Ÿ�����ֹ���));					
			   }				   
			}
			
		   // Ȳ���� ���� ����	   
            else if (emperor !is null)
			{	
			  pk::scene(pk::scene_t(scene_������_õ�ڰ��Ӹ�));				   

               if (0 < loyalist_list.count)
               {	
		   	    pk::scene(pk::scene_t(scene_�ѽ�������_����_��));
			   }

			   if (force.title == ����_�� and Ÿ������_��������)
			   {							  
				pk::scene(pk::scene_t(scene_�հ�����_Ÿ�����ֹ���));					
			   }			   
			}


            pk::scene(pk::scene_t(scene_ȭ��_����));	
			
			
			if (force.title == ����_��)		
			{			
		   // �����丮 �α� ���
			  string duke_title = pk::decode(pk::get_name(pk::get_title(����_��)));		   
		      pk::history_log(pk::get_current_turn_force_id(), force.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� \x1b[1x{}\x1b[0x�� �ڸ��� ������" , pk::decode(pk::get_name(kunshu)), duke_title ) ) );
			}

			}


		} // callback_duke
		


		void scene_������_õ�ھ���()
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

			pk::council(kunshu, subject1, subject2, subject3, subject4, subject5, subject6, pk::council_t(council_������_õ�ھ���_�ߴ�));



			pk::background(30);
			pk::fade(255);
			
            pk::play_se(6);

			string duke_title = pk::decode(pk::get_name(pk::get_title(����_��)));
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ħ�� \x1b[1x{}\x1b[0x�� �ڸ��� �ö���.", kunshuname, duke_title)));
			pk::play_se(10);
			pk::cutin(21);
			pk::play_se(35); 			


		   // ���� ����

		      force.title = ����_��;
		      force.update();

            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x ������ ���ϵ帳�ϴ�.", duke_title)), subject1);
			pk::message_box(pk::encode("�������� ��� �Բ� ������� �����̴�."), kunshu);
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x ������ �̸��� �ɰ�, �����ε� ���� �����ϰڽ��ϴ�.", duke_title)), subject4);			
			pk::message_box(pk::encode("����鵵 ���� �� ����ؾ߰ڽ��ϴ�."), subject5);			
			pk::message_box(pk::encode("��, �ٵ� �����ε� �� ��Ź�Ѵ�. �״�鵵 ������ �� ��Ź�Ѵ�."), kunshu);
            pk::message_box(pk::encode(pk::format("���������ʽÿ�. \x1b[1x{}\x1b[0x�� ���϶�� �̸��� �ɸ°� �ּ��� ���ϰڽ��ϴ�.", duke_title)), subject6);			
			pk::message_box(pk::encode("���� �а����ϰڽ��ϴ�."), subject3);			
			pk::message_box(pk::encode("�ٽ� �ѹ� ���ϵ帳�ϴ�."), subject2);

			pk::background(38);

                                           string current_year = pk::get_year();	
                                           string current_month = pk::get_month();
                                           string current_day = pk::get_day();			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x��.\n\x1b[1x{}\x1b[0x(��)�� \x1b[1x{}\x1b[0x�� ������ ��Ī�߽��ϴ�.", current_year, current_month, current_day, kunshuname, duke_title )));
					
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		
		} // void scene_������_õ�ھ���()



		void council_������_õ�ھ���_�ߴ�()
		{

			pk::person@ subject1 = pk::is_alive(senior1) ? senior1 : pk::get_person(����_����);
			pk::person@ subject2 = pk::is_alive(senior2) ? senior2 : pk::get_person(����_����);
			pk::person@ subject3 = pk::is_alive(senior3) ? senior3 : pk::get_person(����_����);

			pk::person@ subject4 = pk::is_alive(senior4) ? senior4 : pk::get_person(����_����);
			pk::person@ subject5 = pk::is_alive(senior5) ? senior5 : pk::get_person(����_��ȣ);
			pk::person@ subject6 = pk::is_alive(senior6) ? senior6 : pk::get_person(����_����);

			string kunshuname = pk::decode(pk::get_name(kunshu));
			string subject1name = pk::decode(pk::get_name(subject1));
			string duke_title = pk::decode(pk::get_name(pk::get_title(����_��)));			
			
            pk::message_box(pk::encode(pk::format("�ְ�, �ְ��� ������ ���ؿ� �˷��� �� ������ õ�ϸ� �ڵ��� �ֽ��ϴ�. �ε� ���� �Ź��� ���� �� ��Ƹ��ż� \x1b[1x{}\x1b[0x�� ������ �ֽʽÿ�.", duke_title)), subject1);			
            pk::message_box(pk::encode(pk::format("�׷����ϴ�. ���� \x1b[1x{}\x1b[0x�� ������ �� �Դϴ�.", duke_title)), subject2);			
			pk::message_box(pk::encode("�̴� ���������մϴ�. �ְ��� ���� �ɸ´� ���ο� ������ �����ž� �մϴ�."), subject3);			
            pk::message_box(pk::encode(pk::format("�������� ��뿡 �����ϱ� ���ؼ� ���ݺ��� \x1b[1x{}\x1b[0x�� ������ �����ڳ��. �̷��� ������ �Źε鿡�� �����϶�.", duke_title)), kunshu);				



		} // council_������_õ�ھ���_�ߴ�





		void scene_������_õ�ڰ��Ӹ�()
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
			string duke_title = pk::decode(pk::get_name(pk::get_title(����_��)));
			string king_title = pk::decode(pk::get_name(pk::get_title(����_��)));			
			
			pk::move_screen(building.get_pos());
            pk::play_bgm(3);
			pk::message_box(pk::encode(pk::format("��� ��, \x1b[1x{}\x1b[0x���� Ȳ���� ���ڰ� ã�ƿԴ�.", kunshuname)));
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x��, \x1b[1x{}\x1b[0x�����κ��� \x1b[1x{}\x1b[0x\x1b[1x{}\x1b[0x ������ Ư�簡 �Խ��ϴ�.", kunshuname, emperorname, king_title, duke_title)), pk::get_person(����_����));

			pk::diplomacy(emperor, null, kunshu, pk::diplomacy_t(diplomacy_������_õ�ڿ͵���));



			pk::background(30);
			pk::fade(255);	
			
            pk::play_se(6);

			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ħ�� \x1b[1x{}\x1b[0x�� �ڸ��� �ö���.", kunshuname, duke_title)));
			pk::play_se(10);
			pk::cutin(21);
			pk::play_se(35); 			

		   // ���� ����

		      force.title = ����_��;
		      force.update();

            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x ������ ���ϵ帳�ϴ�.", duke_title)), subject1);
			pk::message_box(pk::encode("�������� ��� �Բ� ������� �����̴�."), kunshu);
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x ������ �̸��� �ɰ�, �����ε� ���� �����ϰڽ��ϴ�.", duke_title)), subject4);			
			pk::message_box(pk::encode("����鵵 ���� �� ����ؾ߰ڽ��ϴ�."), subject5);			
			pk::message_box(pk::encode("��, �ٵ� �����ε� �� ��Ź�Ѵ�. �״�鵵 ������ �� ��Ź�Ѵ�."), kunshu);
            pk::message_box(pk::encode(pk::format("���������ʽÿ�. \x1b[1x{}\x1b[0x�� ���϶�� �̸��� �ɸ°� �ּ��� ���ϰڽ��ϴ�.", duke_title)), subject6);			
			pk::message_box(pk::encode("���� �а����ϰڽ��ϴ�."), subject3);			
			pk::message_box(pk::encode("�ٽ� �ѹ� ���ϵ帳�ϴ�."), subject2);


			pk::background(34);
			
                                           string current_year = pk::get_year();	
                                           string current_month = pk::get_month();
                                           string current_day = pk::get_day();			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x��.\n\x1b[1x{}\x1b[0x(��)�� \x1b[1x{}\x1b[0x�κ��� \x1b[1x{}\x1b[0x�� ������ ���������ϴ�.", current_year, current_month, current_day, kunshuname, emperorname, duke_title )));
					
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		
		} // void scene_������_õ�ڰ��Ӹ�()



		void diplomacy_������_õ�ڿ͵���()
		{

			string kunshuname = pk::decode(pk::get_name(kunshu));
			string duke_title = pk::decode(pk::get_name(pk::get_title(����_��)));			
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x, �״븦 \x1b[1x{}\x1b[0x�� ���ϳ��. �����ε� \x1b[1x��\x1b[0x���� ���� �����϶�.", kunshuname, duke_title )), emperor);
			pk::message_box(pk::encode("�����մϴ�. ������ ��뿡 ������ �� �ֵ��� �����ε� ������ �ϰڽ��ϴ�."), kunshu);


		} // diplomacy_������_õ�ڿ͵���()



		void scene_�ѽ�������_����_��()
		{
				@force = pk::get_force(kunshu.get_force_id());

			string kunshuname = pk::decode(pk::get_name(kunshu));
			string duke_title = pk::decode(pk::get_name(pk::get_title(����_��)));	
			
               if (kunshu.kanshitsu == �ѽ�_�߽�)
			   {
			   pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x ���� �ҽĿ� �ѽ��� �߽��ϴ� �Ϻ� ���ŵ��� ������...", kunshuname, duke_title )));	
               pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x���� �и� �ѽ��� ���Ϸμ� �漺�� �� �Ͻ� ���̴�.", kunshuname )), maverick1);		   
				  
			   }

               else if (kunshu.kanshitsu != �ѽ�_�߽�)
			   {
			   pk::message_box(pk::encode(pk::format("������, \x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x ���� �ҽĿ� �ѽ��� �߽��ϴ� �Ϻ� ���ŵ��� ������...", kunshuname, duke_title )));	
               pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x���� \x1b[1x{}\x1b[0x�� �����Ͻ� ���� ��� ��������... ����� ���� ������ ���� ���� ��ɵ� �� ũ�� �þ�� ���� �ڿ����� ��ġ��...�ѽ��� ���Ϸμ��� �ڰ��� ���� �����ž� �����ε�...���� ������ ������ ������...", kunshuname, duke_title )), maverick1);
			   }
			   
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		
		} // void scene_�ѽ�������_����_��()




        //----------------------------------------------------------------------------------
        //  �� ���� �̺�Ʈ
        //----------------------------------------------------------------------------------


// �� ���� �̺�Ʈ

		void callback_king(pk::force@ force)
		{
                                  @emperor = pk::get_person(pk::get_scenario().emperor);

		      if (!force.is_player()) return;
		      if (force.title != ����_��) return;
		      if (pk::get_city_count(force) < king_city_count ) return;

			
			if (force.is_player() and force.title == ����_�� and pk::get_city_count(force) >= king_city_count)
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



           // Ȳ�� ���� ���� ���� ����
                                 pk::list<pk::person@> pro_dethrone_list;
                                 pro_dethrone_list.clear();								 
                                 auto radical_list = pk::list_to_array(pk::get_person_list(force, pk::mibun_flags(�ź�_����,�ź�_�¼�,�ź�_�Ϲ�) ) );

                                 if (0 < radical_list.length)
                                  {
           	                      for (int k = 0; k < radical_list.length; k++)
           	                      {
	                       pk::person@ radical = radical_list[k];	
                                     if (pk::is_alive(radical) and radical.get_id() != kunshu.get_id() and radical.kanshitsu == �ѽ�_����) 	
                                     pro_dethrone_list.add(radical);

	                       }

                                 }

                       if (0 == pro_dethrone_list.count)
                       {

                                   @pro_dethrone1 = pk::get_person(����_����);
                                   @pro_dethrone2 = pk::get_person(����_����);	
                                   @pro_dethrone3 = pk::get_person(����_����);	
                                   @pro_dethrone4 = pk::get_person(����_����);	
                                   @pro_dethrone5 = pk::get_person(����_��ȣ);	
                                   @pro_dethrone6 = pk::get_person(����_����);
								   
	                   }

                       else if (0 < pro_dethrone_list.count)
                       {
	                      pro_dethrone_list.sort(function(a, b)
	                      {				
	                      return float(( a.loyalty * 30 ) / (1 +  (4 * a.rank))) > float(( b.loyalty * 30 ) / (1 + (4 * b.rank)));
 
	                      });

                                   @pro_dethrone1 = 0 < pro_dethrone_list.count ? pro_dethrone_list[0] : pk::get_person(����_����);
                                   @pro_dethrone2 = 1 < pro_dethrone_list.count ? pro_dethrone_list[1] : pk::get_person(����_����);	
                                   @pro_dethrone3 = 2 < pro_dethrone_list.count ? pro_dethrone_list[2] : pk::get_person(����_����);	
                                   @pro_dethrone4 = 3 < pro_dethrone_list.count ? pro_dethrone_list[3] : pk::get_person(����_����);	
                                   @pro_dethrone5 = 4 < pro_dethrone_list.count ? pro_dethrone_list[4] : pk::get_person(����_��ȣ);	
                                   @pro_dethrone6 = 5 < pro_dethrone_list.count ? pro_dethrone_list[5] : pk::get_person(����_����);

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


		   // ���� �ҽĿ� �а��� �ѽ� �߽� ����

		    pk::list<pk::force@> pro_han_force_list;
            pro_han_force_list.clear();			
			for (int i = 0; i < force_list.count; i++)
			{
	        pk::force@ pro_han_force = force_list[i];
	        pk::person@ pro_han_kunshu_group = pk::get_person(pk::get_kunshu_id(pro_han_force));
            if (pro_han_force.get_id() != force.get_id() and pro_han_kunshu_group.kanshitsu == �ѽ�_�߽�) 	
            pro_han_force_list.add(pro_han_force);
			}

            if (pro_han_force_list.count > 0)
		    {
	           pro_han_force_list.sort(function(a, b)
	           {				
	            return pk::get_city_count(a) > pk::get_city_count(b);
	           });
	                    
              @ pro_han_force_1 = pro_han_force_list[0];
              @ pro_han_kunshu = pk::get_person(pk::get_kunshu_id(pro_han_force_1));	
		      @ pro_han_building = pk::get_building(pro_han_kunshu.service);
		    }

		   // �̺�Ʈ ���� 
		   // Ȳ���� ������		   
			if (emperor is null)
			{		   
		      pk::scene(pk::scene_t(scene_������_õ�ھ���));
			  
               if (0 < loyalist_list.count)
               {	
		   	    pk::scene(pk::scene_t(scene_�ѽ�������_����));
			   }

			   if (force.title == ����_�� and Ÿ������_��������)
			   {							  
				pk::scene(pk::scene_t(scene_�հ�����_Ÿ�����ֹ���));					
			   }				   
			}
			
		   // Ȳ���� ���� ����	   
            else if (emperor !is null)
			{	
			  pk::scene(pk::scene_t(scene_������_õ�ڰ��Ӹ�));				   

               if (0 < loyalist_list.count)
               {	
		   	    pk::scene(pk::scene_t(scene_�ѽ�������_����));
			   }

			   if (force.title == ����_�� and Ÿ������_��������)
			   {							  
				pk::scene(pk::scene_t(scene_�հ�����_Ÿ�����ֹ���));					
			   }			   
			}


            pk::scene(pk::scene_t(scene_ȭ��_����));	
			
			
			if (force.title == ����_��)		
			{			
		   // �����丮 �α� ���
			string king_title = pk::decode(pk::get_name(pk::get_title(����_��)));		   
		      pk::history_log(pk::get_current_turn_force_id(), force.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� \x1b[1x{}\x1b[0x�� �ڸ��� ������" , pk::decode(pk::get_name(kunshu)), king_title ) ) );
			}

			}


		} // callback_king
		


		void scene_������_õ�ھ���()
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

			pk::council(kunshu, subject1, subject2, subject3, subject4, subject5, subject6, pk::council_t(council_������_õ�ھ���_�ߴ�));



			pk::background(30);
			pk::fade(255);
			
            pk::play_se(6);

			string king_title = pk::decode(pk::get_name(pk::get_title(����_��)));
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ħ�� \x1b[1x{}\x1b[0x�� �ڸ��� �ö���.", kunshuname, king_title)));
			pk::play_se(10);
			pk::cutin(21);
			pk::play_se(35); 			


		   // ���� ����

		      force.title = ����_��;
		      force.update();

            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x ������ ���ϵ帳�ϴ�.", king_title)), subject1);
			pk::message_box(pk::encode("�������� ��� �Բ� ������� �����̴�."), kunshu);
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x ������ �̸��� �ɰ�, �����ε� ���� �����ϰڽ��ϴ�.", king_title)), subject4);			
			pk::message_box(pk::encode("����鵵 ���� �� ����ؾ߰ڽ��ϴ�."), subject5);			
			pk::message_box(pk::encode("��, �ٵ� �����ε� �� ��Ź�Ѵ�. �״�鵵 ������ �� ��Ź�Ѵ�."), kunshu);
            pk::message_box(pk::encode(pk::format("���������ʽÿ�. \x1b[1x{}\x1b[0x�� ���϶�� �̸��� �ɸ°� �ּ��� ���ϰڽ��ϴ�.", king_title)), subject6);			
			pk::message_box(pk::encode("���� �а����ϰڽ��ϴ�."), subject3);			
			pk::message_box(pk::encode("�ٽ� �ѹ� ���ϵ帳�ϴ�."), subject2);

			pk::background(38);

                                           string current_year = pk::get_year();	
                                           string current_month = pk::get_month();
                                           string current_day = pk::get_day();			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x��.\n\x1b[1x{}\x1b[0x(��)�� \x1b[1x{}\x1b[0x�� ������ ��Ī�߽��ϴ�.", current_year, current_month, current_day, kunshuname, king_title )));
					
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		
		} // void scene_������_õ�ھ���()



		void council_������_õ�ھ���_�ߴ�()
		{

			pk::person@ subject1 = pk::is_alive(senior1) ? senior1 : pk::get_person(����_����);
			pk::person@ subject2 = pk::is_alive(senior2) ? senior2 : pk::get_person(����_����);
			pk::person@ subject3 = pk::is_alive(senior3) ? senior3 : pk::get_person(����_����);

			pk::person@ subject4 = pk::is_alive(senior4) ? senior4 : pk::get_person(����_����);
			pk::person@ subject5 = pk::is_alive(senior5) ? senior5 : pk::get_person(����_��ȣ);
			pk::person@ subject6 = pk::is_alive(senior6) ? senior6 : pk::get_person(����_����);

			string kunshuname = pk::decode(pk::get_name(kunshu));
			string subject1name = pk::decode(pk::get_name(subject1));
			string king_title = pk::decode(pk::get_name(pk::get_title(����_��)));
			
            pk::message_box(pk::encode(pk::format("�ְ�, �ְ��� ������ ���ؿ� �˷��� �� ������ õ�ϸ� �ڵ��� �ֽ��ϴ�. �ε� ���� �Ź��� ���� �� ��Ƹ��ż� \x1b[1x{}\x1b[0x�� ������ �ֽʽÿ�.", king_title)), subject1);			
            pk::message_box(pk::encode(pk::format("�׷����ϴ�. ���� \x1b[1x{}\x1b[0x�� ������ �� �Դϴ�.", king_title)), subject2);			
			pk::message_box(pk::encode("�̴� ���������մϴ�. �ְ��� ���� �ɸ´� ���ο� ������ �����ž� �մϴ�."), subject3);			
            pk::message_box(pk::encode(pk::format("�������� ��뿡 �����ϱ� ���ؼ� ���ݺ��� \x1b[1x{}\x1b[0x�� ������ �����ڳ��. �̷��� ������ �Źε鿡�� �����϶�.", king_title)), kunshu);			
			
		} // council_������_õ�ھ���_�ߴ�





		void scene_������_õ�ڰ��Ӹ�()
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
			string duke_title = pk::decode(pk::get_name(pk::get_title(����_��)));			
			string king_title = pk::decode(pk::get_name(pk::get_title(����_��)));			
	
			
			pk::move_screen(building.get_pos());
            pk::play_bgm(3);
			pk::message_box(pk::encode(pk::format("��� ��, \x1b[1x{}\x1b[0x���� Ȳ���� ���ڰ� ã�ƿԴ�.", kunshuname)));
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x��, \x1b[1x{}\x1b[0x�����κ��� \x1b[1x{}\x1b[0x\x1b[1x{}\x1b[0x ������ Ư�簡 �Խ��ϴ�.", kunshuname, emperorname, king_title, duke_title)), pk::get_person(����_����));
			
			pk::diplomacy(emperor, null, kunshu, pk::diplomacy_t(diplomacy_������_õ�ڿ͵���));



			pk::background(30);
			pk::fade(255);	
			
            pk::play_se(6);

			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ħ�� \x1b[1x{}\x1b[0x�� �ڸ��� �ö���.", kunshuname, king_title)));
			pk::play_se(10);
			pk::cutin(21);
			pk::play_se(35); 			

		   // ���� ����

		      force.title = ����_��;
		      force.update();

            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x ������ ���ϵ帳�ϴ�.", king_title)), subject1);
			pk::message_box(pk::encode("�������� ��� �Բ� ������� �����̴�."), kunshu);
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x ������ �̸��� �ɰ�, �����ε� ���� �����ϰڽ��ϴ�.", king_title)), subject4);			
			pk::message_box(pk::encode("����鵵 ���� �� ����ؾ߰ڽ��ϴ�."), subject5);			
			pk::message_box(pk::encode("��, �ٵ� �����ε� �� ��Ź�Ѵ�. �״�鵵 ������ �� ��Ź�Ѵ�."), kunshu);
            pk::message_box(pk::encode(pk::format("���������ʽÿ�. \x1b[1x{}\x1b[0x�� ���϶�� �̸��� �ɸ°� �ּ��� ���ϰڽ��ϴ�.", king_title)), subject6);			
			pk::message_box(pk::encode("���� �а����ϰڽ��ϴ�."), subject3);			
			pk::message_box(pk::encode("�ٽ� �ѹ� ���ϵ帳�ϴ�."), subject2);


			pk::background(34);
			
                                           string current_year = pk::get_year();	
                                           string current_month = pk::get_month();
                                           string current_day = pk::get_day();			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x��.\n\x1b[1x{}\x1b[0x(��)�� \x1b[1x{}\x1b[0x�κ��� \n\x1b[1x{}\x1b[0x�� ������ ���������ϴ�.", current_year, current_month, current_day, kunshuname, emperorname, king_title )));
					
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		
		} // void scene_������_õ�ڰ��Ӹ�()



		void diplomacy_������_õ�ڿ͵���()
		{

			string kunshuname = pk::decode(pk::get_name(kunshu));
			string king_title = pk::decode(pk::get_name(pk::get_title(����_��)));
			
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x, �״븦 \x1b[1x{}\x1b[0x�� ���ϳ��. �����ε� \x1b[1x��\x1b[0x���� ���� �����϶�.", kunshuname, king_title )), emperor);
			pk::message_box(pk::encode("�����մϴ�. ������ ��뿡 ������ �� �ֵ��� �����ε� ������ �ϰڽ��ϴ�."), kunshu);


		} // diplomacy_������_õ�ڿ͵���()



		void scene_�ѽ�������_����()
		{
				@force = pk::get_force(kunshu.get_force_id());

			string kunshuname = pk::decode(pk::get_name(kunshu));
			string king_title = pk::decode(pk::get_name(pk::get_title(����_��)));
			
               if (kunshu.kanshitsu == �ѽ�_�߽�)
			   {
			   pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x ���� �ҽĿ� �ѽ��� �߽��ϴ� �Ϻ� ���ŵ��� ������...", kunshuname, king_title )));	
               pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x���� �и� �ѽ��� ���Ϸμ� �漺�� �� �Ͻ� ���̴�.", kunshuname )), maverick1);		   
				  
			   }

               else if (kunshu.kanshitsu != �ѽ�_�߽�)
			   {
			   pk::message_box(pk::encode(pk::format("������, \x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x ���� �ҽĿ� �ѽ��� �߽��ϴ� �Ϻ� ���ŵ��� ������...", kunshuname, king_title )));	
               pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x���� \x1b[1x{}\x1b[0x�� �����Ͻ� ���� ��� ��������... ����� ���� ������ ���� ���� ��ɵ� �� ũ�� �þ�� ���� �ڿ����� ��ġ��...�ѽ��� ���Ϸμ��� �ڰ��� ���� �����ž� �����ε�...���� ������ ������ ������...", kunshuname, king_title )), maverick1);
			   }
			   
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		
		} // void scene_�ѽ�������_����()




        //----------------------------------------------------------------------------------
        //  Ȳ�� ���� �̺�Ʈ
        //----------------------------------------------------------------------------------




// Ȳ�� ���� �̺�Ʈ


		void callback_emperor(pk::force@ force)
		{
                                  @emperor = pk::get_person(pk::get_scenario().emperor);

		      if (!force.is_player()) return;
		      if (force.title != ����_��) return;
		      if (pk::get_city_count(force) < emperor_city_count ) return;

			
		  if (force.is_player() and force.title == ����_�� and pk::get_city_count(force) >= emperor_city_count)
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


           // Ȳ�� ���� ���� ���� ����
                                 pk::list<pk::person@> pro_dethrone_list;
                                 pro_dethrone_list.clear();								 
                                 auto radical_list = pk::list_to_array(pk::get_person_list(force, pk::mibun_flags(�ź�_����,�ź�_�¼�,�ź�_�Ϲ�) ) );

                                 if (0 < radical_list.length)
                                  {
           	                      for (int k = 0; k < radical_list.length; k++)
           	                      {
	                       pk::person@ radical = radical_list[k];	
                                     if (pk::is_alive(radical) and radical.get_id() != kunshu.get_id() and radical.kanshitsu == �ѽ�_����) 	
                                     pro_dethrone_list.add(radical);

	                       }

                                 }


                       if (0 == pro_dethrone_list.count)
                       {

                                   @pro_dethrone1 = pk::get_person(����_����);
                                   @pro_dethrone2 = pk::get_person(����_����);	
                                   @pro_dethrone3 = pk::get_person(����_����);	
                                   @pro_dethrone4 = pk::get_person(����_����);	
                                   @pro_dethrone5 = pk::get_person(����_��ȣ);	
                                   @pro_dethrone6 = pk::get_person(����_����);
								   
	                   }

                       else if (0 < pro_dethrone_list.count)
                       {
	                      pro_dethrone_list.sort(function(a, b)
	                      {				
	                      return float(( a.loyalty * 30 ) / (1 +  (4 * a.rank))) > float(( b.loyalty * 30 ) / (1 + (4 * b.rank)));
 
	                      });

                                   @pro_dethrone1 = 0 < pro_dethrone_list.count ? pro_dethrone_list[0] : pk::get_person(����_����);
                                   @pro_dethrone2 = 1 < pro_dethrone_list.count ? pro_dethrone_list[1] : pk::get_person(����_����);	
                                   @pro_dethrone3 = 2 < pro_dethrone_list.count ? pro_dethrone_list[2] : pk::get_person(����_����);	
                                   @pro_dethrone4 = 3 < pro_dethrone_list.count ? pro_dethrone_list[3] : pk::get_person(����_����);	
                                   @pro_dethrone5 = 4 < pro_dethrone_list.count ? pro_dethrone_list[4] : pk::get_person(����_��ȣ);	
                                   @pro_dethrone6 = 5 < pro_dethrone_list.count ? pro_dethrone_list[5] : pk::get_person(����_����);

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


		   // ���� �ҽĿ� �а��� �ѽ� �߽� ����

		    pk::list<pk::force@> pro_han_force_list;
            pro_han_force_list.clear();			
			for (int i = 0; i < force_list.count; i++)
			{
	        pk::force@ pro_han_force = force_list[i];
	        pk::person@ pro_han_kunshu_group = pk::get_person(pk::get_kunshu_id(pro_han_force));
            if (pro_han_force.get_id() != force.get_id() and pro_han_kunshu_group.kanshitsu == �ѽ�_�߽�) 	
            pro_han_force_list.add(pro_han_force);
			}

            if (pro_han_force_list.count > 0)
		    {
	           pro_han_force_list.sort(function(a, b)
	           {				
	            return pk::get_city_count(a) > pk::get_city_count(b);
	           });
	                    
              @ pro_han_force_1 = pro_han_force_list[0];
              @ pro_han_kunshu = pk::get_person(pk::get_kunshu_id(pro_han_force_1));	
		      @ pro_han_building = pk::get_building(pro_han_kunshu.service);
			  		  			  
		    }

		   // �̺�Ʈ ���� 
		   // Ȳ���� ������		   
			if (emperor is null)
			{		   
		      pk::scene(pk::scene_t(scene_Ȳ������_õ�ھ���));
			  
			          if (force.title == ����_Ȳ�� and Ÿ������_��������)
			          {							  										   			   
						  pk::scene(pk::scene_t(scene_Ȳ������_Ÿ�����ֹ���));					
			          }				  
			}
			
		   // Ȳ���� ���� �ְ� ���ְ� �˸���	   
            else if (emperor !is null and pk::is_protecting_the_emperor(force))
			{	
		   // ���ְ� �ѽ� �߽��̸鼭 Ȳ���� �ֿ켱 ģ�ֹ����� ��� ���� 		
               if ( (����_���� == 0 and kunshu.kanshitsu == �ѽ�_�߽� and pk::is_like(emperor, kunshu.get_id())) or (����_���� == 1 and kunshu.kanshitsu == �ѽ�_�߽�) )
			   {
				      pk::scene(pk::scene_t(scene_Ȳ������_õ�ڰ�����));
					  
			          if (force.title == ����_Ȳ�� and Ÿ������_��������)
			          {							  
						  pk::scene(pk::scene_t(scene_Ȳ������_Ÿ�����ֹ���));					
			          }					  
			   }
		   // �׿� �ٸ� ����...���ְ� �ѽ� �߽ð� �ƴϰų� Ȳ���� ģ�ֹ����� �ƴ� ��� ���� ����			   
               else
			   {
				      pk::scene(pk::scene_t(scene_Ȳ������_õ�ڸ���������));
					  
			          if (force.title == ����_Ȳ�� and Ÿ������_��������)
			          {							  
                         if (emperor.mibun == �ź�_���)
			              {	
			  	          pk::scene(pk::scene_t(scene_Ȳ��ó��_Ÿ�����ֹ���));						  
			              }	
                         else
						  pk::scene(pk::scene_t(scene_Ȳ������_Ÿ�����ֹ���));
					
			           }						  
					  
			          if (����������_�÷��̾�_����)
					      pk::scene(pk::scene_t(scene_�ѽ���������_����));


			   }			   

			}

            pk::scene(pk::scene_t(scene_ȭ��_����));

			if (force.title == ����_Ȳ��)		
			{
		  			   
		   // �����丮 �α� ���
			  string emperor_title = pk::decode(pk::get_name(pk::get_title(����_Ȳ��)));
		      pk::history_log(pk::get_current_turn_force_id(), force.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� \x1b[1x{}\x1b[0x�� ���" , pk::decode(pk::get_name(kunshu)), emperor_title ) ) );
		      pk::history_log(pk::get_current_turn_force_id(), force.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[2x{}\x1b[0x(��)�� ����" , pk::decode(pk::get_name(kunshu))  , pk::decode(pk::get_name(pk::get_kokugou(force.kokugou))) ) ) );
			}

		  }


		} // callback_emperor
		
	
		
		void scene_Ȳ������_õ�ھ���()
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

			pk::council(kunshu, subject1, subject2, subject3, subject4, subject5, subject6, pk::council_t(council_Ȳ������_õ�ھ���_����));




		   // ��ȣ ���� ����


			pk::background(13);
			pk::fade(255);
			pk::message_box(pk::encode("�ְ�, ��ȣ�� \x1b[1x��\x1b[0x�� ����Ͻðڽ��ϱ�?"), subject1);

				array<string> answers =
				{
					pk::encode("\x1b[1x��\x1b[0x�� ����Ѵ�."),
					pk::encode("���ο� ��ȣ�� ���Ѵ�.")
				};
				
				int n = pk::choose(pk::encode("��ȣ�� ���� ��ħ�� �����Ͻʽÿ�."), answers);
		if (n == 0) 
                                   kokugou_han();

		if (n == 1) 
                  {
			  pk::message_box(pk::encode("�׷��ٸ� ���ο� ��ȣ�� ���ϼž� �� �� �մϴ�, ����."), subject1);					  
		      kokugou_0();

				  if (�����߽ð���_�漺����)
                     {
                                    // ���� �� �ѿս� ��ȣ�����ڵ� �漺�� ��ȭ
                                 pk::list<pk::person@> loyalist_list;
                                 loyalist_list.clear();									 
                                 auto member_list = pk::list_to_array(pk::get_person_list(force, pk::mibun_flags(�ź�_����,�ź�_�¼�,�ź�_�Ϲ�) ) );

                                 if (0 < member_list.length)
                                  {
           	                      for (int j = 0; j < member_list.length; j++)
           	                      {
	                       pk::person@ loyalist = member_list[j];	
                                     if (pk::is_alive(loyalist) and loyalist.get_id() != kunshu.get_id() and loyalist.kanshitsu == �ѽ�_�߽�
									 and loyalist.get_id() != subject1.get_id() and loyalist.get_id() != subject2.get_id() and loyalist.get_id() != subject3.get_id()
									 and loyalist.get_id() != subject4.get_id() and loyalist.get_id() != subject5.get_id() and loyalist.get_id() != subject6.get_id()) 	
           	                        {
			// ���ָ� ģ���ϰų� �κ�, ������, �θ��ڽ� ������ ��� �������� ����
			if (func_relation_check(loyalist, kunshu) == 0 and func_mutual_relation_check(loyalist, kunshu) == 0)
           	                        {
                                       loyalist_list.add(loyalist);
		          pk::add_loyalty(loyalist, -��ȣ����_�漺����);

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

			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x�� ���ο� ��ȣ�� ������ ������ ����� �޾�, ������ �߽��ϴ� ��ŵ��� �漺���� �����Ͽ���.", kunshuname )));	
			pk::message_box(pk::encode("��....�̰� ���� ���ΰ�! �ְ�! \x1b[1x��\x1b[0x�� ����� �ٽ� �ǻ츮�ž� �մϴ�. �ٽ� �ѹ� ������!"), maverick1);
	                   }

                     } 

                } 

			pk::fade(0);
			pk::sleep();
			pk::background(18);	
			pk::fade(255);
            pk::play_bgm(19);			

            pk::play_se(6);
			
			string emperor_title = pk::decode(pk::get_name(pk::get_title(����_Ȳ��))); 
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ħ�� \x1b[1x{}\x1b[0x�� �ڸ��� �ö���.", kunshuname, emperor_title)));
			pk::play_se(10);
			pk::cutin(21);
			pk::play_se(35);			

		   // ���� ����

		      force.title = ����_Ȳ��;
		      force.update();

			pk::message_box(pk::encode("�� ��� ���� �� �����е� �����̿�."), kunshu);
			pk::message_box(pk::encode("����帮�ɴϴ�, ����!"), subject1);
			pk::message_box(pk::encode("�� �鼺�鵵 ȣ���ϰ� �ֽ��ϴ�, ����!"), subject4);
			pk::message_box(pk::encode("�����ε� �����е��� �� ���� �����ֽÿ�."), kunshu);
			pk::message_box(pk::encode("���ϸ� ���� ���� ����� ������ ���� �� �ְڽ��ϱ�!"), subject5);
			pk::message_box(pk::encode("���� õ�������̶�� �������� ���ұ���."), subject3);
			pk::message_box(pk::encode("������ ������ ���ؿ� �θ� ��ġ�ڽ��ϴ�. ����帮�ɴϴ�."), subject6);
			pk::message_box(pk::encode("�ٽ� �ѹ� ����帮�ɴϴ�."), subject2);


			pk::background(19);
	
                                           string current_year = pk::get_year();	
                                           string current_month = pk::get_month();
                                           string current_day = pk::get_day();			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x��.\n\x1b[1x{}\x1b[0x(��)�� ��ħ�� \x1b[1x{}\x1b[0x�� �ڸ��� �ö���.\n��ȣ�� \x1b[1x{}\x1b[0x.", current_year, current_month, current_day, kunshuname, emperor_title , pk::decode(pk::get_name(pk::get_kokugou(force.kokugou))) )));
					
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		
		} // void scene_Ȳ������_õ�ھ���()
		
		

		void council_Ȳ������_õ�ھ���_����()
		{

			pk::person@ subject1 = pk::is_alive(senior1) ? senior1 : pk::get_person(����_����);
			pk::person@ subject2 = pk::is_alive(senior2) ? senior2 : pk::get_person(����_����);
			pk::person@ subject3 = pk::is_alive(senior3) ? senior3 : pk::get_person(����_����);

			pk::person@ subject4 = pk::is_alive(senior4) ? senior4 : pk::get_person(����_����);
			pk::person@ subject5 = pk::is_alive(senior5) ? senior5 : pk::get_person(����_��ȣ);
			pk::person@ subject6 = pk::is_alive(senior6) ? senior6 : pk::get_person(����_����);

			string kunshuname = pk::decode(pk::get_name(kunshu));
			string subject1name = pk::decode(pk::get_name(subject1));
			string emperor_title = pk::decode(pk::get_name(pk::get_title(����_Ȳ��))); 
			
            pk::message_box(pk::encode(pk::format("�ְ�, \x1b[1x{}\x1b[0x�� �ڸ��� �����ʽÿ�.", emperor_title)), subject1);				
			

			pk::message_box(pk::encode("�׷���ɴϴ�. ������ �����Ҽ�."), subject3);
			pk::message_box(pk::encode("�ƴ�, �� ���� ���̿�? ���� �׷� �� ����..."), kunshu);
			pk::message_box(pk::encode("�ְ�..."), subject1);
			pk::message_box(pk::encode("�̴� ���� ���ο� ���� ������ �ƴϿɴϴ�..."), subject2);
			pk::message_box(pk::encode("õ�ϰ� ���ο� ������ ���ϰ� �ְ� �ְ��� ���� ������ ����� �� ���ŵ� ���� �ٶ�� ���̿ɴϴ�."), subject1); 
			pk::message_box(pk::encode("�̷��� ��뿡 �������� ���Ѵٸ� ������ ū �츦 ���ϰ� �� ���Դϴ�..."), subject3); 
			pk::message_box(pk::encode("��...."), kunshu);
				
            pk::message_box(pk::encode(pk::format("�� �׷��ٸ� �� ���� �� ���� �ְڼ�... �˰ڼ�... �� \x1b[1x{}\x1b[0x�� �ǰڼ�.", emperor_title)), kunshu);
			
			pk::message_box(pk::encode("�����̿ɴϱ�! �� �����ϼ̽��ϴ�, �ְ�!"), subject1);
			pk::message_box(pk::encode("�� ��� �ҽ��� ���� õ�Ͽ� �˷��߰ڱ���!"), subject3);
			pk::message_box(pk::encode("��� ����鿡�� ū ���� �� ���Դϴ�!"), subject2);

		} // council_Ȳ������_õ�ھ���_����()



		void scene_Ȳ������_õ�ڰ�����()
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

	
			
			pk::move_screen(building.get_pos());
            pk::play_bgm(3);
			pk::message_box(pk::encode(pk::format("��� ��, \x1b[1x{}\x1b[0x���� Ȳ���� ���ڰ� ã�ƿԴ�.", kunshuname)));
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x��, Ȳ������ ã���ʴϴ�." , kunshuname)), pk::get_person(����_����));
			pk::message_box(pk::encode("���ڱ� ���ϲ���...?"), kunshu);


			pk::diplomacy(emperor, null, kunshu, pk::diplomacy_t(diplomacy_õ�ڰ�������������));



		   // ��ȣ ���� ����


			pk::background(13);
			pk::fade(255);
			pk::message_box(pk::encode("�ְ�, ��ȣ�� \x1b[1x��\x1b[0x�� ����Ͻðڽ��ϱ�?"), subject1);

				array<string> answers =
				{
					pk::encode("\x1b[1x��\x1b[0x�� ����Ѵ�."),
					pk::encode("���ο� ��ȣ�� ���Ѵ�.")
				};
				
				int n = pk::choose(pk::encode("��ȣ�� ���� ��ħ�� �����Ͻʽÿ�."), answers);
		if (n == 0) 
                                   kokugou_han();

		if (n == 1) 
                  {	
			  pk::message_box(pk::encode("�׷��ٸ� ���ο� ��ȣ�� ���ϼž� �� �� �մϴ�, ����."), subject1);			  
		      kokugou_0();

				  if (�����߽ð���_�漺����)
                     {
                                    // ���� �� �ѿս� ��ȣ�����ڵ� �漺�� ��ȭ
                                 pk::list<pk::person@> loyalist_list;
                                 loyalist_list.clear();									 
                                 auto member_list = pk::list_to_array(pk::get_person_list(force, pk::mibun_flags(�ź�_����,�ź�_�¼�,�ź�_�Ϲ�) ) );

                                 if (0 < member_list.length)
                                  {
           	                      for (int j = 0; j < member_list.length; j++)
           	                      {
	                       pk::person@ loyalist = member_list[j];	
                                     if (pk::is_alive(loyalist) and loyalist.get_id() != kunshu.get_id() and loyalist.kanshitsu == �ѽ�_�߽�
									 and loyalist.get_id() != subject1.get_id() and loyalist.get_id() != subject2.get_id() and loyalist.get_id() != subject3.get_id()
									 and loyalist.get_id() != subject4.get_id() and loyalist.get_id() != subject5.get_id() and loyalist.get_id() != subject6.get_id()) 	
           	                        {
			// ���ָ� ģ���ϰų� �κ�, ������, �θ��ڽ� ������ ��� �������� ����
			if (func_relation_check(loyalist, kunshu) == 0 and func_mutual_relation_check(loyalist, kunshu) == 0)
           	                        {
                                       loyalist_list.add(loyalist);
		          pk::add_loyalty(loyalist, -��ȣ����_�漺����_����);

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

			pk::message_box(pk::encode(pk::format("��� \x1b[1x{}\x1b[0x�� ���ο� ��ȣ�� ������ ������ ������ �߽��ϴ� ��ŵ鿡�� ȣ���� ���� ��������...", kunshuname )));	
			pk::message_box(pk::encode("....��Ÿ������ ���ϲ��� ���� \x1b[1x��\x1b[0x�� ����� ���������ŰŴ�, �޾Ƶ��̴� �� �ۿ�..."), maverick1);
			pk::message_box(pk::encode(pk::format("��ȭ���� �������� ������, \x1b[1x{}\x1b[0x�� ���ο� ��ȣ�� ũ�� �ݹ��ϴ� ��ŵ��� �����ٰ� �Ѵ�...", kunshuname )));		

                       }
                     } 

                } 

			pk::fade(0);
			pk::sleep();
			pk::background(18);
			pk::fade(255);			
            pk::play_bgm(19);

            pk::play_se(6);

			string emperor_title = pk::decode(pk::get_name(pk::get_title(����_Ȳ��))); 
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ħ�� \x1b[1x{}\x1b[0x�� �ڸ��� �ö���.", kunshuname, emperor_title)));
			pk::play_se(10);
			pk::cutin(21);
			pk::play_se(35); 			


			// ���� Ȳ�� ����
		
            pk::get_scenario().emperor = ����_��;
            emperor.update();


		   // ���� ����

		      force.title = ����_Ȳ��;
		      force.update();

			pk::message_box(pk::encode("�� ��� ���� �� �����е� �����̿�."), kunshu);
			pk::message_box(pk::encode("����帮�ɴϴ�, ����!"), subject1);
			pk::message_box(pk::encode("�� �鼺�鵵 ȣ���ϰ� �ֽ��ϴ�, ����!"), subject4);
			pk::message_box(pk::encode("�����ε� �����е��� �� ���� �����ֽÿ�."), kunshu);
			pk::message_box(pk::encode("���ϸ� ���� ���� ����� ������ ���� �� �ְڽ��ϱ�!"), subject5);
			pk::message_box(pk::encode("���� õ�������̶�� �������� ���ұ���."), subject3);
			pk::message_box(pk::encode("������ ������ ���ؿ� �θ� ��ġ�ڽ��ϴ�. ����帮�ɴϴ�."), subject6);
			pk::message_box(pk::encode("�ٽ� �ѹ� ����帮�ɴϴ�."), subject2);


			pk::background(19);

                                           string current_year = pk::get_year();	
                                           string current_month = pk::get_month();
                                           string current_day = pk::get_day();			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x��.\n\x1b[1x{}\x1b[0x(��)�� ��ħ�� \x1b[1x{}\x1b[0x�� �ڸ��� �ö���.\n��ȣ�� \x1b[1x{}\x1b[0x.", current_year, current_month, current_day, kunshuname, kunshuname , pk::decode(pk::get_name(pk::get_kokugou(force.kokugou))) )));
					
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		
		} // void scene_Ȳ������_õ�ڰ�����()


		void diplomacy_õ�ڰ�������������()
		{
			pk::person@ subject1 = pk::is_alive(senior1) ? senior1 : pk::get_person(����_����);
			pk::person@ subject2 = pk::is_alive(senior2) ? senior2 : pk::get_person(����_����);
			pk::person@ subject3 = pk::is_alive(senior3) ? senior3 : pk::get_person(����_����);

			pk::person@ subject4 = pk::is_alive(senior4) ? senior4 : pk::get_person(����_����);
			pk::person@ subject5 = pk::is_alive(senior5) ? senior5 : pk::get_person(����_��ȣ);
			pk::person@ subject6 = pk::is_alive(senior6) ? senior6 : pk::get_person(����_����);

			string kunshuname = pk::decode(pk::get_name(kunshu));
			string subject1name = pk::decode(pk::get_name(subject1));			

			pk::message_box(pk::encode("����, �� ã���̴� ������ϴ�."), kunshu);
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x��! �� ���־���. �� �״뿡�� ���� ���� �ּ�.", kunshuname)), emperor);

            pk::message_box(pk::encode(pk::format("���� \x1b[1x{}\x1b[0x��, �״뿡�� ������ �Ϸ� �Ͽ�.", kunshuname)), emperor);
			pk::message_box(pk::encode("�ƴ�...����, ���ڱ� ���� �����̽���..."), kunshu);
			
            pk::message_box(pk::encode(pk::format("���� �������� �״�, \x1b[1x{}\x1b[0x���� ����� ����, �״밡 �� ��� �� ���� ������ų ���������� Ȯ���߼�. �ε� �� ������ �˾��ֱ� �ٶ��.", kunshuname)), emperor);			
			
			pk::message_box(pk::encode("�ְ�, õ�ڱ��� �ɻ���� ���� ������ �����̽ô� ������ �����ʽÿ�."), subject1);
			pk::message_box(pk::encode("�½��ϴ�. �ε� õ�ϸ� ������Ű�� ���� �����!"), subject2);
			pk::message_box(pk::encode("��...."), kunshu);
									
			pk::message_box(pk::encode("��¿ �� ������...������ �濡 �����ڽ��ϴ�."), kunshu);
			pk::message_box(pk::encode("�� �����ϼ̼�! ���� õ�ϰ� ����ϰڱ���."), emperor);
			pk::message_box(pk::encode("�� ��� �ҽ��� ���� õ�Ͽ� �˷��߰ڱ���!"), subject1);
			pk::message_box(pk::encode("��� ����鿡�� ū ���� �� ���Դϴ�!"), subject2);

		} // diplomacy_õ�ڰ�������������()


		void scene_Ȳ������_õ�ڸ���������()
		{
				@force = pk::get_force(kunshu.get_force_id());
			pk::person@ subject1 = pk::is_alive(pro_dethrone1) ? pro_dethrone1 : pk::get_person(����_����);
			pk::person@ subject2 = pk::is_alive(pro_dethrone2) ? pro_dethrone2 : pk::get_person(����_����);
			pk::person@ subject3 = pk::is_alive(pro_dethrone3) ? pro_dethrone3 : pk::get_person(����_����);

			pk::person@ subject4 = pk::is_alive(pro_dethrone4) ? pro_dethrone4 : pk::get_person(����_����);
			pk::person@ subject5 = pk::is_alive(pro_dethrone5) ? pro_dethrone5 : pk::get_person(����_��ȣ);
			pk::person@ subject6 = pk::is_alive(pro_dethrone6) ? pro_dethrone6 : pk::get_person(����_����);

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

			pk::fade(0);
			pk::sleep();
			pk::background(53);
			pk::fade(255);
			
            pk::play_bgm(15);
			
			string emperor_title = pk::decode(pk::get_name(pk::get_title(����_Ȳ��))); 
									
			pk::message_box(pk::encode("�ְ�, ������ õ�ڸ� ���Ͻð� ���� Ȳ���� �����ʽÿ�."), subject1);
			pk::message_box(pk::encode("�ƴ�, �װ� ���� �Ҹ���?"), kunshu);			
            pk::message_box(pk::encode(pk::format("�ְ��� ������ ���ظ� ��ġ�� ������, õ�� ������ ����� ��������� �ᱹ \x1b[1x{}\x1b[0x�� �����ϼž� �մϴ�.", emperor_title)), subject2);						
			pk::message_box(pk::encode("�׷��� �ص�...�ʹ��� ���۽��� �̾߱Ⱑ �ƴϿ�?"), kunshu);		
            pk::message_box(pk::encode(pk::format("�ְ����� õ���� 6�� �����̸� �����ϼ����� \x1b[1x{}\x1b[0x�� �ڸ��� �������� �ʴ´ٸ�, �ᱹ�� ���� õ������, �ְ��� õ�ϰ� �ƴ� ���Դϴ�.", emperor_title)), subject3);			
			pk::message_box(pk::encode("......."), kunshu);	
			pk::message_box(pk::encode("�� �ð��� ��ü�ϼ̴ٰ� ������ õ���� �ڸ��� ���������ϴ�. �ְ�, �ε� �����..."), subject4);
			pk::message_box(pk::encode("���� �ô�� ��������, ���� ���ο� �ô�� ���ư��� �մϴ�."), subject5);			
			pk::message_box(pk::encode("�½��ϴ�. õ�Ͽ� ���ο� ������ ���ĺ����� �մϴ�. �ְ�, � �����!"), subject6);				
			
			
			
				array<string> response =
				{
					pk::encode("�ƹ��� ������ ������ �ʴ´�."),
					pk::encode("õ�ڸ� �����ϰ�, ������ ���� ������ ������.")
				};
				
				int m = pk::choose(pk::encode("õ�ڸ� �����ϰ� ������ Ȳ���� ���� ���θ� �����Ͻʽÿ�."), response);
				
		if (m == 0) 
		   {			
                                   õ�ڰ�������_ħ��();
								   
			if (Ȳ������_�ŷ°���) 
		     {
                 pk::list<pk::person@> target_list;
                 target_list.clear();				 
		         pk::person@ target_member;	
				 array<string> person_name_list(0);

				 if (pk::is_alive(pro_dethrone1))
				 target_list.add(pro_dethrone1);	

				 if (pk::is_alive(pro_dethrone2))
				 target_list.add(pro_dethrone2);	
			 
				 if (pk::is_alive(pro_dethrone3))
				 target_list.add(pro_dethrone3);

                if (target_list.count > 0)
		     	{
           	       for (int j = 0; j < target_list.count; j++)
           	       {
					@target_member = target_list[j];  

                     if (target_member.base_stat[����ɷ�_�ŷ�] > ����_�ŷ�_���ҷ�)
		     	     {
				     target_member.base_stat[����ɷ�_�ŷ�] = target_member.base_stat[����ɷ�_�ŷ�] - ����_�ŷ�_���ҷ�;	
                     target_member.update();
		             }
                     else if (target_member.base_stat[����ɷ�_�ŷ�] <= ����_�ŷ�_���ҷ�)
		     	     {
				     target_member.base_stat[����ɷ�_�ŷ�] = 1;	
                     target_member.update();
		             }	
					 
				  person_name_list.insertLast(pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(target_member))));
		 					   
		           }
				   
				  string person_names = join(person_name_list, ", "); 
		        		
			     pk::play_se(11);
			     pk::message_box(pk::encode(pk::format("Ȳ���� ������ �밡�� \x1b[2x{}\x1b[0x - �� \x1b[2x{}\x1b[0x���� ������ ����, �ŷ��� �����Ͽ����ϴ�.", person_names, target_list.count)));		
		        }
					 
		      }								   
		   }

		if (m == 1) 
		   {			
                                   õ�ڰ�������_����ó��();			

				if (Ȳ������_�ŷ°���) 
		     	{	
			     pk::play_se(11);
			     pk::message_box(pk::encode(pk::format("Ȳ���� ������ �밡�� \x1b[2x{}\x1b[0x�� ������ ����, �ŷ��� �����Ͽ����ϴ�.", kunshuname)));		

                     if (kunshu.base_stat[����ɷ�_�ŷ�] > ����_�ŷ�_���ҷ�)
		     	     {
				     kunshu.base_stat[����ɷ�_�ŷ�] = kunshu.base_stat[����ɷ�_�ŷ�] - ����_�ŷ�_���ҷ�;	
                     kunshu.update();
		             }
                     else if (kunshu.base_stat[����ɷ�_�ŷ�] <= ����_�ŷ�_���ҷ�)
		     	     {
				     kunshu.base_stat[����ɷ�_�ŷ�] = 1;	
                     kunshu.update();
		             }
						 
		        }			
		   }




		   // ��ȣ ���� ����

			pk::background(41);	
			pk::fade(255);
			pk::message_box(pk::encode("����, \x1b[1x��\x1b[0x�� ���� ���� ������ ����� �����̿ɴϴ�."), subject1);

			pk::message_box(pk::encode("�׷��ٸ� ���ο� ��ȣ�� ���ϼž� �� �� �մϴ�, ����."), subject2);
		      kokugou_0();
			  
			  
			pk::fade(0);
			pk::sleep();
			pk::background(18);
			pk::fade(255);

            pk::play_bgm(22);							 

            pk::play_se(6);
			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ħ�� \x1b[1x{}\x1b[0x�� �ڸ��� �ö���.", kunshuname, emperor_title)));
			pk::play_se(10);

			pk::cutin(21);
			pk::play_se(35); 

			// ���� Ȳ�� ���� + Ȳ���� �÷��̾� ���ָ� �����ϱ� ����
			
		    if (!pk::is_dislike(emperor, kunshu.get_id()) and pk::get_num_dislikes(emperor) <= 4)
            pk::add_dislike(emperor, kunshu.get_id());				
		
            pk::get_scenario().emperor = ����_��;
            emperor.update();


		   // ���� ����

		      force.title = ����_Ȳ��;
		      force.update();

			pk::message_box(pk::encode("�� ��� ���� �� �����е� �����̿�."), kunshu);
			pk::message_box(pk::encode("����帮�ɴϴ�, ����!"), subject1);
			pk::message_box(pk::encode("�� �鼺�鵵 ȣ���ϰ� �ֽ��ϴ�, ����!"), subject4);
			pk::message_box(pk::encode("�����ε� �����е��� �� ���� �����ֽÿ�."), kunshu);
			pk::message_box(pk::encode("���ϸ� ���� ���� ����� ������ ���� �� �ְڽ��ϱ�!"), subject5);
			pk::message_box(pk::encode("���� õ�������̶�� �������� ���ұ���."), subject3);
			pk::message_box(pk::encode("������ ������ ���ؿ� �θ� ��ġ�ڽ��ϴ�. ����帮�ɴϴ�."), subject6);
			pk::message_box(pk::encode("�ٽ� �ѹ� ����帮�ɴϴ�."), subject2);


			pk::background(19);

                                           string current_year = pk::get_year();	
                                           string current_month = pk::get_month();
                                           string current_day = pk::get_day();			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x��.\n\x1b[1x{}\x1b[0x(��)�� ��ħ�� \x1b[1x{}\x1b[0x�� �ڸ��� �ö���.\n��ȣ�� \x1b[1x{}\x1b[0x.", current_year, current_month, current_day, kunshuname, emperor_title , pk::decode(pk::get_name(pk::get_kokugou(force.kokugou))) )));

			pk::message_box(pk::encode("������� ������ ���� ���뿡�� ���ϴٴ�...�������� ���� ������ �� �� ������...�β�������..."), emperor);

					
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);



		if (Ȳ��_ó��) 
		   {			

			string emperorname = pk::decode(pk::get_name(emperor));			

   
			pk::move_screen(building.get_pos());
            pk::play_bgm(14);
			pk::message_box(pk::encode(pk::format("������, �̰����� ���� ���� �ƴϾ���...\x1b[1x{}\x1b[0x���� \x1b[2x{}\x1b[0x(��)�� ����� ���ŵ��� �� ������� ������ �ϰ� �Ǵµ�...", kunshuname, subject1name)));
			pk::message_box(pk::encode("����, ������ Ȳ���� ��� �Ͻðڽ��ϱ�...?"), subject1);

			pk::council(kunshu, subject1, subject2, subject3, subject4, subject5, subject6, pk::council_t(council_õ��ó��_����));
	
	

				array<string> execution =
				{
					pk::encode("�ϴ� ������ Ȳ���� ����д�."),
					pk::encode("������ Ȳ���� ó���Ͽ� ��ȯ�� ���´�.")
				};
				
				int k = pk::choose(pk::encode("õ�ڿ� ���� ó�츦 �����Ͻʽÿ�."), execution);

		if (k == 0) 
		   {			
			pk::message_box(pk::encode("���� �׷��Ա��� �ش������� �ൿ�� �ʿ䰡 �ְڼ�? �׸��� ������ ������ ������ �ٲ������ ������ ������Ű�� �� �޼�����. ���� ������ ����."), kunshu);
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x�� �� Ȳ���� �ϴ� ����α�� �ϸ�, \x1b[2x{}\x1b[0x(��)�� ����� ���ŵ��� ������ �����ƴ�.", kunshuname, subject1name)));			

		   }			

		if (k == 1) 
		   {			
			pk::message_box(pk::encode("...�´� ���̿�. ��ȯ�� ���� �����ص־߰���..."), kunshu);
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x�� \x1b[2x{}\x1b[0x(��)�� ����� ���ŵ��� ������� �� Ȳ���� ó���ϱ�� �����ߴ�.", kunshuname, subject1name)));			

			pk::fade(0);
			pk::sleep();
			pk::background(55);
			pk::fade(255);

            pk::message_box(pk::encode(pk::format("�̷� ����...��� �ڽ��� �� ���ָ� ���̷� �Ѵ� ���ΰ�, \x1b[1x{}\x1b[0x!...", kunshuname )), emperor);
			pk::message_box(pk::encode("�ȵ����� ��¿ �� �����̴�."), subject1);
            pk::message_box(pk::encode(pk::format("�ƾ�...���� ������ ���翡 �� ������� �������ڴٴ� �ǰ�...\x1b[1x{}\x1b[0x, �� ���� ���� ��!", kunshuname )), emperor);
			pk::message_box(pk::encode("���μ��� � �����."), subject1);	
			pk::message_box(pk::encode("����!"), pk::get_person(����_����));	
			pk::cutin(15);	
            pk::message_box(pk::encode(pk::format("���� �״��� ���� ����� ���簡 �� ������ ������ ���̴�! \x1b[1x{}\x1b[0x!", kunshuname )), emperor);			

                                           string current_year = pk::get_year();	
                                           string current_month = pk::get_month();
                                           string current_day = pk::get_day();			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x��.\n\x1b[1x{}\x1b[0x, \x1b[1x{}\x1b[0x���� ó�����ϴ�.", current_year, current_month, current_day, emperorname, kunshuname )));
			pk::play_se(5);	
            pk::kill(emperor, kunshu, pk::get_hex_object(emperor.location), null, 2);			
					
				if (Ȳ��ó��_�ŷ°���)
		     	{	
			     pk::play_se(11);
			     pk::message_box(pk::encode(pk::format("Ȳ���� ó���� �밡�� \x1b[2x{}\x1b[0x�� ������ ����, �ŷ��� �����Ͽ����ϴ�.", kunshuname)));		

                     if (kunshu.base_stat[����ɷ�_�ŷ�] > ó��_�ŷ�_���ҷ�)
		     	     {
				     kunshu.base_stat[����ɷ�_�ŷ�] = kunshu.base_stat[����ɷ�_�ŷ�] - ó��_�ŷ�_���ҷ�;	
                     kunshu.update();
		             }
                     else if (kunshu.base_stat[����ɷ�_�ŷ�] <= ó��_�ŷ�_���ҷ�)
		     	     {
				     kunshu.base_stat[����ɷ�_�ŷ�] = 1;	
                     kunshu.update();
		             }
						 
		        }					 
		   }			
 
			
  
		   }




				  if (�����߽ð���_�漺����)
                     {						 
                                    // ���� �� �ѿս� ��ȣ�����ڵ� �漺�� ��ȭ
                                 pk::list<pk::person@> loyalist_list;
                                 loyalist_list.clear();									 
                                 auto member_list = pk::list_to_array(pk::get_person_list(force, pk::mibun_flags(�ź�_����,�ź�_�¼�,�ź�_�Ϲ�) ) );

                                 if (0 < member_list.length)
                                  {
           	                      for (int j = 0; j < member_list.length; j++)
           	                      {
	                       pk::person@ loyalist = member_list[j];	
                                     if (pk::is_alive(loyalist) and loyalist.get_id() != kunshu.get_id() and loyalist.kanshitsu == �ѽ�_�߽�) 	
           	                        {
			// ���ָ� ģ���ϰų� �κ�, ������, �θ��ڽ� ������ ��� �������� ����
			if (func_relation_check(loyalist, kunshu) == 0 and func_mutual_relation_check(loyalist, kunshu) == 0
				and loyalist.get_id() != subject1.get_id() and loyalist.get_id() != subject2.get_id() and loyalist.get_id() != subject3.get_id()
				and loyalist.get_id() != subject4.get_id() and loyalist.get_id() != subject5.get_id() and loyalist.get_id() != subject6.get_id())
           	                        {
                                       loyalist_list.add(loyalist);
                                    if (emperor.mibun != �ź�_���)									   
		                                pk::add_loyalty(loyalist, -����_�漺����);
									
                                    else if (emperor.mibun == �ź�_���)									
		                                pk::add_loyalty(loyalist, -������ó��_�漺����);

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
                                    if (emperor.mibun != �ź�_���)
           	                        {										
									pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x�� �� ������ �����Ų �ҽĿ� ������ �߽��ϴ� ��ŵ��� �г밡 �������.", kunshuname )));	
									pk::message_box(pk::encode("��� �̷� ���� ���� �� �ִ� ���ΰ�! ���� ��ô� ������ ���� �����Ű�ٴ�!...."), maverick1);
                                    }

                                    else if (emperor.mibun == �ź�_���)
          	                        {										
									pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x�� �� ������ �����Ų �͵� ���ڶ� Ȳ���� ó���ߴٴ� �ҽĿ� ������ �߽��ϴ� ��ŵ��� �г�� ����� ġ�ھҴ�.", kunshuname )));	
									pk::message_box(pk::encode("��..�̰� ���� �Ǵ°�? ���� ��ô� ������ ���� �����Ų �͵� ���ڶ� Ȳ���� ó���ϴٴ�!...."), maverick1);
                                    }
                       }									
                     } 	

		


		} // void scene_Ȳ������_õ�ڸ���������()



		void council_õ��ó��_����()
		{
				@force = pk::get_force(kunshu.get_force_id());			
			pk::person@ subject1 = pk::is_alive(pro_dethrone1) ? pro_dethrone1 : pk::get_person(����_����);
			pk::person@ subject2 = pk::is_alive(pro_dethrone2) ? pro_dethrone2 : pk::get_person(����_����);
			pk::person@ subject3 = pk::is_alive(pro_dethrone3) ? pro_dethrone3 : pk::get_person(����_����);

			pk::person@ subject4 = pk::is_alive(pro_dethrone4) ? pro_dethrone4 : pk::get_person(����_����);
			pk::person@ subject5 = pk::is_alive(pro_dethrone5) ? pro_dethrone5 : pk::get_person(����_��ȣ);
			pk::person@ subject6 = pk::is_alive(pro_dethrone6) ? pro_dethrone6 : pk::get_person(����_����);

			string kunshuname = pk::decode(pk::get_name(kunshu));
			string subject1name = pk::decode(pk::get_name(subject1));
			
			pk::message_box(pk::encode("����, �� Ȳ���� �״�� �θ� ��ȯ�� ����� �ηƽ��ϴ�."), subject1);
			pk::message_box(pk::encode("�׷���ɴϴ�. �и� �� �ѽǿ� ���� �̷��� ���� �ڵ��� �˸��Ϸ� �� ���� �ֻ�ɴϴ�."), subject2);		
			pk::message_box(pk::encode("�׷����� �������� �ʹ� �����ϴ� ���� �ƴϿ�? ���� ������ �� Ȳ���� ���� �������� ���� �͵� �ƴ��ݼ�."), kunshu);			
			pk::message_box(pk::encode("�㳪 �� Ȳ���� ��� �����Ѵٴ� ��� �����ε� �ѽ������ڵ鿡�� �ٸ� ������ ǰ�� �� ������ �ְ� �Ǵ� �̴ϴ�."), subject3);
			pk::message_box(pk::encode("�׸��� ���ϲ����� �ƽôٽ��� �׵鿡�� ���� �߿��� ���� ��оƴϰڽ��ϱ�."), subject4);			
			pk::message_box(pk::encode("....��..."), kunshu);
			pk::message_box(pk::encode("�ᱹ �׵��� ���ϲ��� õ�ϸ� �����Ϸ��� �߿��� �ñ⿡ �� Ȳ���� ������ ��翡 ������ �� ���Դϴ�."), subject5);
			pk::message_box(pk::encode("����, �ΰ����� ������ ������ �ѱ�� ���� �ƴմϴ�. ���� ��ģ �迡 �ݶ��� ���� Ȯ���� �����ؾ� �մϴ�."), subject6);				

		} // void council_õ��ó��_����()


		void õ�ڰ�������_ħ��()
		{	
				@force = pk::get_force(kunshu.get_force_id());
			pk::person@ subject1 = pk::is_alive(pro_dethrone1) ? pro_dethrone1 : pk::get_person(����_����);	
			pk::person@ subject2 = pk::is_alive(pro_dethrone2) ? pro_dethrone2 : pk::get_person(����_����);
			pk::person@ subject3 = pk::is_alive(pro_dethrone3) ? pro_dethrone3 : pk::get_person(����_����);
			
			string kunshuname = pk::decode(pk::get_name(kunshu));
			string subject1name = pk::decode(pk::get_name(subject1));
			
				pk::message_box(pk::encode("......."), kunshu);
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x�� �ƹ��� ���� ���� �ʴ´�...", kunshuname)));					

			pk::message_box(pk::encode("�ְ�, �������� ���ʽÿ�. ���� �˾Ƽ� �ϰڽ��ϴ�."), subject1);
			pk::message_box(pk::encode("�� ���� ���񿡰� �������ֽñ� �ٶ��ϴ�."), subject2);
			pk::message_box(pk::encode("�ְ������� ������ ���Ѻ��ñ⸸ �ϸ� �˴ϴ�."), subject3);			

			pk::fade(0);
			pk::sleep();
			pk::background(34);	
			pk::fade(255);			
			


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
			 		 
                if (target_list.count > 0)
		     	{
           	       for (int j = 0; j < target_list.count; j++)
           	       {
					@target_member = target_list[j];  
				  
				  person_name_list.insertLast(pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(target_member))));
		 					   
		           }
				   		        				
		        }

				  string person_names = join(person_name_list, ", "); 	

			
			pk::message_box(pk::encode(pk::format("�� ��, \x1b[2x{}\x1b[0x - �̵� \x1b[1x{}\x1b[0x�� ���ŵ��� Ȳ���� �����ߴ�.", person_names, kunshuname)));

			pk::council(emperor, pk::get_person(����_����), subject1, subject2, subject3, null, null, pk::council_t(council_õ�ڰ�������_���ŵ�õ������));

		} // õ�ڰ�������_ħ��()


		void council_õ�ڰ�������_���ŵ�õ������()
		{	
			pk::person@ subject1 = pk::is_alive(pro_dethrone1) ? pro_dethrone1 : pk::get_person(����_����);	
			pk::person@ subject2 = pk::is_alive(pro_dethrone2) ? pro_dethrone2 : pk::get_person(����_����);
			pk::person@ subject3 = pk::is_alive(pro_dethrone3) ? pro_dethrone3 : pk::get_person(����_����);
			
			string kunshuname = pk::decode(pk::get_name(kunshu));			
					  																																																
			pk::message_box(pk::encode("���� ������ �ʹٴ� ���� �����. �׷�, �״���� ���� �Ϸ� �Լ�?"), emperor);	
			pk::message_box(pk::encode(pk::format("����, ���� \x1b[1x{}\x1b[0x�Բ� �ڸ��� �����ֽ��� ��ڽ��ϱ�?" , kunshuname)), subject1);			
			pk::message_box(pk::encode("!!!!"), emperor);
			pk::message_box(pk::encode(pk::format("���� ����� �̹� ���߰�, õ�ϸ� ������ �̾�� ���� ���� \x1b[1x{}\x1b[0x���� ���� Ȳ���� �����ž� �Ѵٰ� ���ϴ�." , kunshuname)), subject2);
			pk::message_box(pk::encode("����! �� ���̶�� �̷� ������ ������ �ϴ� �Կ�!"), pk::get_person(����_����));				
			pk::message_box(pk::encode("�״���� ���� ������ �̷� �� �ִ°�!"), emperor);
			pk::message_box(pk::encode("������ �뼼�� �������� �ô��� ������ �Ž��� �� ���� ���Դϴ�. ���ϲ����� ���� �׸� �����Ͻ�����."), subject3);	
			pk::message_box(pk::encode("..............������� �ϴ��� �η����� �ʴ���..."), emperor);	
			pk::message_box(pk::encode("�� Ȳ�� �ȿ� ����ų �� ���� ���°� �������� �ʴ� �� ���ϽŴٸ�, ������ �޾Ƶ��̽ô� �� ���� �̴ϴ�."), subject1);		
			pk::message_box(pk::encode(".......�˰ڼ�. ��� ���� �Ͻÿ�."), emperor);			
			pk::message_box(pk::encode("���� �� �ϼ̽��ϴ�. �����Ͻ� �ɷ� �˰�, ������ �����ϰڽ��ϴ�."), subject2);	
			pk::message_box(pk::encode("����...."), pk::get_person(����_����));				
			pk::message_box(pk::encode(".......������ ���� ������������ ...."), emperor);				

		} // void council_õ�ڰ�������_���ŵ�õ������()


		void õ�ڰ�������_����ó��()
		{	
				@force = pk::get_force(kunshu.get_force_id());
			pk::person@ subject1 = pk::is_alive(pro_dethrone1) ? pro_dethrone1 : pk::get_person(����_����);	
			pk::person@ subject2 = pk::is_alive(pro_dethrone2) ? pro_dethrone2 : pk::get_person(����_����);
			pk::person@ subject3 = pk::is_alive(pro_dethrone3) ? pro_dethrone3 : pk::get_person(����_����);
			
			string kunshuname = pk::decode(pk::get_name(kunshu));
			string subject1name = pk::decode(pk::get_name(subject1));
            string emperor_title = pk::decode(pk::get_name(pk::get_title(����_Ȳ��)));			
			
				pk::message_box(pk::encode("�˰ڼ��̴�. �� ���� ��������. ���� ���� Ȳ���� ����� ������ �ڰ��� ���ڼ�."), kunshu);
			    pk::message_box(pk::encode(pk::format("���� \x1b[1x{}\x1b[0x�� ���� \x1b[1x{}\x1b[0x�� �ڸ��� ���� ���� ������!", kunshuname, emperor_title)));					

			pk::message_box(pk::encode("������...�� �����ϼ̽��ϴ�, �ְ�!"), subject1);
			pk::message_box(pk::encode("����! ������ õ�Ͽ� ����� �� ���� ���� �ǰڱ���!"), subject2);
			pk::message_box(pk::encode("������ Ȳ�������� �����Ͻǰ̴ϴ�."), subject3);			
						
			pk::fade(0);
			pk::sleep();
			pk::background(34);	
			pk::fade(255);	
			
			pk::message_box(pk::encode(pk::format("�� ��, \x1b[1x{}\x1b[0x�� ���� Ȳ���� �����ߴ�.", kunshuname)));

			pk::diplomacy(emperor, null, kunshu, pk::diplomacy_t(diplomacy_õ�ڸ���������));

		} // õ�ڰ�������_����ó��()	


		void diplomacy_õ�ڸ���������()
		{
		
			string kunshuname = pk::decode(pk::get_name(kunshu));	
			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x��, ����ÿ�. �׷�, ������ ���� �Ϸ�?" , kunshuname)), emperor);				
		    pk::message_box(pk::encode("�ܶ��� ����������, ���ϲ����� ���� õ���� ������ ��� ���� ��ʴϱ�?"), kunshu);
			pk::message_box(pk::encode("õ���� �����̶��?"), emperor);
			pk::message_box(pk::encode(pk::format("���� �� ������ ����� �� ���� ���ϰ� �ְ�, �� \x1b[1x{}\x1b[0x�� ������ �۾��� ����� ������ ���� �����Դϴ�." , kunshuname)), kunshu);				
			pk::message_box(pk::encode("....?"), emperor);
			
			pk::message_box(pk::encode(pk::format("õ���� ������ ���ǰǴ�, ������� �����Ѵٸ� ������ �� \x1b[1x{}\x1b[0x�� õ�ϸ� �ٽ����� ���� ���� �ʰڽ��ϱ�?" , kunshuname)), kunshu);	
			pk::message_box(pk::encode("!!!!"), emperor);			
		    pk::message_box(pk::encode("���ϲ��� ���� õ�ϸ� �����ϽŴٸ�, õ�ϸ� ���� ���ο��Է� �����ֽø��� �����մϴ�."), kunshu);				
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x��, �� ���� ��ġ�� ���� �Ҹ���! ���� ����...���� ������ڴ� ���̿�?! ���� �׷��� �� �� ����!" , kunshuname)), emperor);		
		    pk::message_box(pk::encode("������ ������ ġ��ġ�� ���ð�, �ʳ��� �ϵ��� �� �����Ͻñ� �ٶ��ϴ�."), kunshu);		
			pk::message_box(pk::encode(pk::format("�ƾ�...\x1b[1x{}\x1b[0x, ����� ����� �� ������ ���ʶ߸��� �Ϸ��°�..." , kunshuname)), emperor);	
		    pk::message_box(pk::encode("õ���� ������ �ٲ�� ���� �ƴϿ�? ���� ����� ���������̴�."), kunshu);			
			
			pk::message_box(pk::encode("........�˰ڼ�. �״� ���� �Ͻÿ�..."), emperor);			
		    pk::message_box(pk::encode("�� �����ϼ̼��̴�. �׷� �̸�."), kunshu);
			pk::message_box(pk::encode("......�����ϵ���...."), emperor);			

		} // void diplomacy_õ�ڸ���������()



        //----------------------------------------------------------------------------------
        //  ��ȣ ���� â
        //----------------------------------------------------------------------------------



// ��ȣ ����

		void kokugou_han()
		{	
				@force = pk::get_force(kunshu.get_force_id());
			pk::person@ subject1 = pk::is_alive(senior1) ? senior1 : pk::get_person(����_����);	
			string kunshuname = pk::decode(pk::get_name(kunshu));

			pk::background(13);	
				pk::message_box(pk::encode("�翬�� �츮 ��� \x1b[1x��\x1b[0x�� �ڽĵ� �ƴϰڼ�. �� ������ ��ȣ�� \x1b[1x��\x1b[0x���� �����ϰ� �ѿ����� ����ϰڼ�!"), kunshu);
				force.kokugou = ��ȣ_��; 
		                             force.update();

			pk::message_box(pk::encode("������ �����Ͻ� �����̿ɴϴ�, ����."), subject1);
			pk::background(12);	
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x{}\x1b[0x���� �����ϸ� �ѿս��� ����Ͽ���.", kunshuname , pk::decode(pk::get_name(pk::get_kokugou(force.kokugou))) )));


                  if (�����߽ð���_�漺����)
                  {					  

                                    // ���� �� �ѿս� ��ȣ�����ڵ� �漺�� ��ȭ
                                 pk::list<pk::person@> loyalist_list;
                                 loyalist_list.clear();								 
                                 auto member_list = pk::list_to_array(pk::get_person_list(force, pk::mibun_flags(�ź�_����,�ź�_�¼�,�ź�_�Ϲ�) ) );

                                 if (0 < member_list.length)
                                  {
           	                      for (int j = 0; j < member_list.length; j++)
           	                      {
	                       pk::person@ loyalist = member_list[j];	
                                     if (pk::is_alive(loyalist) and loyalist.get_id() != kunshu.get_id() and loyalist.kanshitsu == �ѽ�_�߽�) 	
           	                        {
                                       loyalist_list.add(loyalist);
		          pk::add_loyalty(loyalist, ��ȣ����_�漺����);
	                        }
	                       }

                                 }

                       if (0 < loyalist_list.count)
                       {
	                      loyalist_list.sort(function(a, b)
	                      {				
	                      return a.rank < b.rank;
				
	                      if (a.rank == b.rank)				
	                      return (a.loyalty > b.loyalty);
	                      });

                                   @maverick1 = loyalist_list[0];

			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x�� ��ȣ ���� �ҽĿ� ������ �߽��ϴ� ��ŵ��� �漺���� �����Ͽ���.", kunshuname )));
			pk::message_box(pk::encode("���ϼ̽��ϴ�! �ְ�! \x1b[1x��\x1b[0x�� ����� �̷��� �ٽ� ��Ƴ��� ����! �̷��� �����Ҽ���!"), maverick1);
                       }
                    }
	
		} // kokugou_han()


			
		void kokugou_0()
		{	
			pk::background(41);		
				@force = pk::get_force(kunshu.get_force_id());
			pk::person@ subject1 = pk::is_alive(senior1) ? senior1 : pk::get_person(����_����);	
			string kunshuname = pk::decode(pk::get_name(kunshu));

			array<string> kokugou_list =
			{
				pk::encode("��, ��, ��, ��, ��"),
				pk::encode("��, ��, ��, ��, ��"),
				pk::encode("��, ��, ��, ��, ��"),
				pk::encode("��, ��, ��, ��, ��"),
				pk::encode("��, ��, ä, ��, ��"),
				pk::encode("��, ��, ��, ��, ��"),
			};
			
			int n = pk::choose(pk::encode("��ȣ�� �������ֽʽÿ�."), kokugou_list);
			
			if (n == 0)
			{
				kokugou_1();
			}
			else if (n == 1)
			{
				kokugou_2();
			}
			else if (n == 2)
			{
				kokugou_3();
			}
			else if (n == 3)
			{
				kokugou_4(); 
			}
			else if (n == 4)
			{
				kokugou_5();
			}
			else if (n == 5)
			{
				kokugou_6();
			}



		} // kokugou_0()
		
		void kokugou_1()
		{
			string kunshuname = pk::decode(pk::get_name(kunshu));			
			array<string> kokugou_list =
			{
				pk::encode("��"),
				pk::encode("��"),
				pk::encode("��"),
				pk::encode("��"),
				pk::encode("��"),
				pk::encode("BACK"),
			};
			
			int n = pk::choose(pk::encode("��ȣ�� �������ֽʽÿ�."), kokugou_list);
			
			if (n == 0)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x�� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x�� �����Ͽ���.", kunshuname  )));				
			    }
		        if (k == 1) 			
				kokugou_1();
			}
			else if (n == 1)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x�� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x�� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_1();
			}
			else if (n == 2)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x���� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x���� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x���� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x���� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_1();
			}
			else if (n == 3)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x���� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x���� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x���� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x���� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_1();
			}
			else if (n == 4)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x���� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x���� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x���� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x���� �����Ͽ���.", kunshuname  )));				
			    }
		        if (k == 1) 			
				kokugou_1();
			}
			else if (n == 5)
			{
				kokugou_0();
			}
			
			
		} // kokugou_1()
		
		void kokugou_2()
		{
			string kunshuname = pk::decode(pk::get_name(kunshu));			
			array<string> kokugou_list =
			{
				pk::encode("��"),
				pk::encode("��"),
				pk::encode("��"),
				pk::encode("��"),
				pk::encode("��"),
				pk::encode("BACK"),
			};
			
			int n = pk::choose(pk::encode("��ȣ�� �������ֽʽÿ�."), kokugou_list);
			
			if (n == 0)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x�� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x�� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_2();
			}
			else if (n == 1)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x���� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x���� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x���� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x���� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_2();
			}
			else if (n == 2)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x�� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x�� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_2();
			}
			else if (n == 3)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x���� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x���� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x���� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x���� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_2();
			}
			else if (n == 4)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x���� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x���� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x���� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x���� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_2();
			}
			else if (n == 5)
			{
				kokugou_0();
			}	
			
		} // kokugou_2()


		void kokugou_3()
		{
			string kunshuname = pk::decode(pk::get_name(kunshu));			
			array<string> kokugou_list =
			{
				pk::encode("��"),
				pk::encode("��"),
				pk::encode("��"),
				pk::encode("��"),
				pk::encode("��"),
				pk::encode("BACK"),
			};
			
			int n = pk::choose(pk::encode("��ȣ�� �������ֽʽÿ�."), kokugou_list);
			
			if (n == 0)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x�� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x�� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_3();
			}
			else if (n == 1)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x�� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x�� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_3();
			}
			else if (n == 2)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x�� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x�� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_3();
			}
			else if (n == 3)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x�� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x�� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_3();
			}
			else if (n == 4)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x�� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x�� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_3();
			}
			else if (n == 5)
			{
				kokugou_0();
			}
			
			
		} // kokugou_3()


		void kokugou_4()
		{
			string kunshuname = pk::decode(pk::get_name(kunshu));			
			array<string> kokugou_list =
			{
				pk::encode("��"),
				pk::encode("��"),
				pk::encode("��"),
				pk::encode("��"),
				pk::encode("��"),
				pk::encode("BACK"),
			};
			
			int n = pk::choose(pk::encode("��ȣ�� �������ֽʽÿ�."), kokugou_list);
			
			if (n == 0)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x�� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x�� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_4();
			}
			else if (n == 1)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x���� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x���� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x���� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x���� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_4();
			}
			else if (n == 2)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x�� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x�� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_4();
			}
			else if (n == 3)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x���� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x���� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x���� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x���� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_4();
			}
			else if (n == 4)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x�� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update();
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x�� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_4();
			}
			else if (n == 5)
			{
				kokugou_0();
			}
			
			
		} // kokugou_4()


		void kokugou_5()
		{
			string kunshuname = pk::decode(pk::get_name(kunshu));			
			array<string> kokugou_list =
			{
				pk::encode("��"),
				pk::encode("��"),
				pk::encode("ä"),
				pk::encode("��"),
				pk::encode("��"),
				pk::encode("BACK"),
			};
			
			int n = pk::choose(pk::encode("��ȣ�� �������ֽʽÿ�."), kokugou_list);
			
			if (n == 0)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x�� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x�� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_5();
			}
			else if (n == 1)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x���� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x���� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x���� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x���� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_5();
			}
			else if (n == 2)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1xä\x1b[0x�� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1xä\x1b[0x�� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1xä\x1b[0x�� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_ä; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1xä\x1b[0x�� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_5();
			}
			else if (n == 3)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x���� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x���� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x���� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update();
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x���� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_5();
			}
			else if (n == 4)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x���� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x���� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x���� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x���� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_5();
			}
			else if (n == 5)
			{
				kokugou_0();
			}
			
			
		} // kokugou_5()


		void kokugou_6()
		{
			string kunshuname = pk::decode(pk::get_name(kunshu));			
			array<string> kokugou_list =
			{
				pk::encode("��"),
				pk::encode("��"),
				pk::encode("��"),
				pk::encode("��"),
				pk::encode("��"),
				pk::encode("BACK"),
			};
			
			int n = pk::choose(pk::encode("��ȣ�� �������ֽʽÿ�."), kokugou_list);
			
			if (n == 0)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x�� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update();
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x�� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_6();
			}
			else if (n == 1)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x�� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update();
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x�� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_6();
			}
			else if (n == 2)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x�� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x�� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_6();
			}
			else if (n == 3)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x���� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x���� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x���� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��2; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x���� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_6();
			}
			else if (n == 4)
			{
				array<string> answers =
				{
					pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ѵ�."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("��ȣ�� \x1b[1x��\x1b[0x�� �Ͻðڽ��ϱ�?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("�� ������ ��ȣ�� \x1b[1x��\x1b[0x�� �����ϳ��!"), kunshu);
				force.kokugou = ��ȣ_��2; 
		        force.update();
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(��)�� ��ȣ�� \x1b[1x��\x1b[0x�� �����Ͽ���.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_6();
			}
			else if (n == 5)
			{
				kokugou_0();
			}
			
			
		} // kokugou_6()



        //----------------------------------------------------------------------------------
        //  �� ���� Ÿ�� ����
        //----------------------------------------------------------------------------------



		void scene_�հ�����_Ÿ�����ֹ���()
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
	
		} // scene_�հ�����_Ÿ�����ֹ���()




        //----------------------------------------------------------------------------------
        //  Ȳ�� ���� Ÿ�� ����
        //----------------------------------------------------------------------------------



		void scene_Ȳ������_Ÿ�����ֹ���()
		{
			string kunshuname = pk::decode(pk::get_name(kunshu));
		   // ���� �ҽĿ� �а��� Ÿ�� ���� ���
            if (pk::is_alive(enemy_kunshu) and pk::is_alive(enemy_force_1)) 	
		    {
             if (enemy_kunshu.get_id() == kunshu.get_id()) 
                 return;
				
			pk::move_screen(enemy_building.get_pos());
            pk::play_bgm(20);
			
            string emperor_title = pk::decode(pk::get_name(pk::get_title(����_Ȳ��)));			
			pk::message_box(pk::encode(pk::format("����, \x1b[1x{}\x1b[0x�� \x1b[1x{}\x1b[0x ���ӿ� �а��� ���� �־�����....", kunshuname, emperor_title)));
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x.... �� ���� ���� ���� ���� �����? �� ���� ���� ������ ���� ���� �� ������!" , kunshuname)), enemy_kunshu);

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

		   // ���� �ҽĿ� �а��� �ѽ� �߽� ���� ���
            if (pk::is_alive(pro_han_kunshu) and pk::is_alive(pro_han_force_1)) 	
		    {
               if (pro_han_kunshu.get_id() == kunshu.get_id()) 
                   return;

		      pk::force@ pro_han_force_group_1;		
		      pk::person@ pro_han_force_group_kunshu;			
			  pk::building@ pro_han_force_group_kunshu_building;	

		      array<string> han_person_name_list(0);
		      pk::list<pk::force@> pro_han_force_group_list;	
              pro_han_force_group_list.clear();			  
			  pk::list<pk::force@> force_list = pk::get_force_list();
			  for (int i = 0; i < force_list.count; i++)
			  {
	              pk::force@ pro_han_force = force_list[i];
	              pk::person@ pro_han_kunshu_group = pk::get_person(pk::get_kunshu_id(pro_han_force));
                  if (pro_han_force.get_id() != force.get_id() and pro_han_kunshu_group.kanshitsu == �ѽ�_�߽�) 	
			      {			
                   pk::play_bgm(20);														
		           pk::building@ pro_han_kunshu_group_building = pk::get_building(pro_han_kunshu_group.service);																						 
			       pk::move_screen(pro_han_kunshu_group_building.get_pos());																											 
			       pk::message_box(pk::encode(pk::format("������ �����! �� \x1b[1x{}\x1b[0x �� �������� ���� ���� ������!" , kunshuname)), pro_han_kunshu_group);
                   pk::add_relations(pro_han_force, force.get_id(), -Ÿ������_��ȣ���϶��� );
                   pro_han_force_group_list.add(pro_han_force);
                   			
                   if (pro_han_force_group_list.count > 0)
		           {
	                 pro_han_force_group_list.sort(function(a, b)
	                 {				
	                return pk::get_city_count(a) > pk::get_city_count(b);
	                 });
	                    
                     @ pro_han_force_group_1 = pro_han_force_group_list[0];
                     @ pro_han_force_group_kunshu = pk::get_person(pk::get_kunshu_id(pro_han_force_group_1));	
		             @ pro_han_force_group_kunshu_building = pk::get_building(pro_han_force_group_kunshu.service);
				 
		           }

			      }			
			  }
			  
                  if (pro_han_force_group_list.count > 0)
		          {			  
           	   	   	 for (int k = 0; k < pro_han_force_group_list.count; k++)
           	   	   	 {		 
	           	   	 pk::force@ target_group = pro_han_force_group_list[k];
               	   	 pk::person@ target_member = pk::get_person(pk::get_kunshu_id(target_group));
			   	   	 han_person_name_list.insertLast(pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(target_member))));	 					   
		       	   	 }			  

			       string pro_han_force_group_kunshuname = pk::decode(pk::get_name(pro_han_force_group_kunshu)); 

                   string person_names = join(han_person_name_list, ", ");

                   pk::message_box(pk::encode(pk::format("���⿡ ������ �߽��ߴ� \x1b[2x{}\x1b[0x - \x1b[2x{}\x1b[0x���� ����(��)�� \x1b[1x{}\x1b[0x�� ���� ��ȣ���� �߰��� �϶��ߴ�.", person_names, pro_han_force_group_list.count, kunshuname)));
 	
				   pk::fade(0);
				   pk::sleep();
				   pk::background(-1);
				   pk::fade(255);	

			    }			  
		    }		

		} // scene_Ȳ������_Ÿ�����ֹ���()



		void scene_Ȳ��ó��_Ÿ�����ֹ���()
		{
			string kunshuname = pk::decode(pk::get_name(kunshu));
		   // ������ ó�� �ҽĿ� �а��� Ÿ�� ���� ���
          if (pk::is_alive(enemy_kunshu) and pk::is_alive(enemy_force_1)) 	
		  {
             if (enemy_kunshu.get_id() == kunshu.get_id()) 
                 return;
							
            if (emperor.mibun == �ź�_���)									   								
		    {
			pk::move_screen(enemy_building.get_pos());
            pk::play_bgm(20);			
			pk::message_box(pk::encode(pk::format("����, \x1b[1x{}\x1b[0x�� Ȳ���� ó���ϰ� ������ �ö��ٴ� �ҽĿ� �а��� ���� �־�����....", kunshuname)));
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x.... �� �ؾǹ����� ��! ����� Ȳ���� ���̰� �߿��� �巯�±���! ���� õ�ϰ� ����� ���δٴ� ���̳�! �㳪 ���� ���ſ���ٰ� ū ȭ�� ��ġ ���� ���̴�!" , kunshuname)), enemy_kunshu);

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
               pk::add_relations(enemy_force_1, force.get_id(), -Ÿ������_��ȣ���϶���_Ȳ��ó�� );				
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
		  }

		   // ������ ó�� �ҽĿ� �а��� �ѽ� �߽� ���� ���
          if (pk::is_alive(pro_han_kunshu) and pk::is_alive(pro_han_force_1)) 	
		  {
               if (pro_han_kunshu.get_id() == kunshu.get_id()) 
                   return;

		    pk::force@ pro_han_force_group_1;		
		    pk::person@ pro_han_force_group_kunshu;			
			pk::building@ pro_han_force_group_kunshu_building;	

		    array<string> han_person_name_list(0);
		    pk::list<pk::force@> pro_han_force_group_list;	
            pro_han_force_group_list.clear();			
			pk::list<pk::force@> force_list = pk::get_force_list();
			for (int i = 0; i < force_list.count; i++)
			{
	           pk::force@ pro_han_force = force_list[i];
	           pk::person@ pro_han_kunshu_group = pk::get_person(pk::get_kunshu_id(pro_han_force));
               if (pro_han_force.get_id() != force.get_id() and pro_han_kunshu_group.kanshitsu == �ѽ�_�߽�) 	
	           {		         										  
                     if (emperor.mibun == �ź�_���)									   								
		             {																									 
                     pk::play_bgm(20);														
		             pk::building@ pro_han_kunshu_group_building = pk::get_building(pro_han_kunshu_group.service);																						 
			         pk::move_screen(pro_han_kunshu_group_building.get_pos());																											 
			         pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x �� ������! ������ �����Ų �͵� ���ڶ� Ȳ���� ���̴� õ�ΰ����� ���� �������ٴ�! �� ������ ���� ���� ���� ���̴�!" , kunshuname)), pro_han_kunshu_group);
                     pk::add_relations(pro_han_force, force.get_id(), -Ÿ������_��ȣ���϶���_Ȳ��ó�� );		                               
                     pro_han_force_group_list.add(pro_han_force);
                   			
                     if (pro_han_force_group_list.count > 0)
		             {
	                   pro_han_force_group_list.sort(function(a, b)
	                   {				
	                  return pk::get_city_count(a) > pk::get_city_count(b);
	                   });
	                    
                       @ pro_han_force_group_1 = pro_han_force_group_list[0];
                       @ pro_han_force_group_kunshu = pk::get_person(pk::get_kunshu_id(pro_han_force_group_1));	
		               @ pro_han_force_group_kunshu_building = pk::get_building(pro_han_force_group_kunshu.service);
					   			 
		             }

		             }						
			   }			
			} 
			
                 if (pro_han_force_group_list.count > 0)
		         {			
           	     	for (int k = 0; k < pro_han_force_group_list.count; k++)
           	     	{		 
	             	pk::force@ target_group = pro_han_force_group_list[k];
                 	pk::person@ target_member = pk::get_person(pk::get_kunshu_id(target_group));
			     	han_person_name_list.insertLast(pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(target_member))));	 					   
		         	}				

			         string pro_han_force_group_kunshuname = pk::decode(pk::get_name(pro_han_force_group_kunshu)); 

                   string person_names = join(han_person_name_list, ", ");

                   pk::message_box(pk::encode(pk::format("���⿡ ������ �߽��ߴ� \x1b[2x{}\x1b[0x - \x1b[2x{}\x1b[0x���� ����(��)�� \x1b[1x{}\x1b[0x�� ���� ��ȣ���� �߰��� �϶��ߴ�.", person_names, pro_han_force_group_list.count, kunshuname)));
          							
						pk::fade(0);
						pk::sleep();
						pk::background(-1);
						pk::fade(255);
			    }						
			
		  }		

		} // scene_Ȳ��ó��_Ÿ�����ֹ���()



        //----------------------------------------------------------------------------------
        //  �ѽ� �����ڵ��� �÷��̾� ���� ���� �߰�
        //----------------------------------------------------------------------------------



		// �ѽ������ڵ��� �������忡 �߰�
		void scene_�ѽ���������_����()
		{
			string kunshuname = pk::decode(pk::get_name(kunshu));
		    pk::person@ hansilman_group_1; 
		    pk::building@ hansilman_group_1_building;
			
            pk::list<pk::person@> hansilman_list;
            hansilman_list.clear();			
			for (int i = 0; i < ����_��; i++)
			{
                pk::person@ hansilman = pk::get_person(i);

				if (pk::is_alive(hansilman) and hansilman.get_id() != kunshu.get_id())
				{	
				  if (hansilman.kanshitsu == �ѽ�_�߽� and !pk::is_dislike(hansilman, kunshu.get_id())
					  and pk::get_num_dislikes(hansilman) < 5 and hansilman.mibun != �ź�_���
				      and func_relation_check(hansilman,kunshu) == 0 and func_mutual_relation_check(hansilman,kunshu) == 0)
				  {	
				   hansilman_list.add(hansilman);
				   pk::add_dislike(hansilman, kunshu.get_id());

                     if (hansilman_list.count > 0)
		             {
	                    hansilman_list.sort(function(a, b)
	                    {				
	                     return a.rank < b.rank;
	                    });
	                    
                       @ hansilman_group_1 = hansilman_list[0];                    	
		               @ hansilman_group_1_building = pk::get_building(hansilman_group_1.service);	  	  
			         }		   
			      }
		       }
		    }

            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x, �� ������! �� õ���� ������ �˸� ������ ¡���ϸ���!" , kunshuname)), hansilman_group_1);																						 
		
			string hansilman_group_1_name = pk::decode(pk::get_name(hansilman_group_1));
 
            pk::message_box(pk::encode(pk::format("\x1b[2x{}\x1b[0x(��)�� ������ \x1b[2x{}\x1b[0x���� ������ �ѽ� �߽� �λ���� \x1b[1x{}\x1b[0x�� �����ϱ� �����ߴ�.", hansilman_group_1_name, hansilman_list.count, kunshuname)));
	 
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);	


		} // scene_�ѽ���������_����()



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