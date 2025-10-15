namespace 이벤트_주자사_부터_대사마_등극
{
	
	// ================ CUSTOMIZE ================
		
        const bool 한실주의자_독백보기 = true;        // 신하들 중 즉위에 반응하는 한실주의자 반응 보기
		const int 한실주의자_독백_시작_작위 = 작위_대장군;   	// 한실주의자 반응이 나오기 시작할 작위 	
	
        const bool 타국군주_반응보기 = true;        // 즉위에 경계하는 타국 군주 반응 발생 여부
		const int 타국군주_반응_시작_작위 = 작위_대장군;   	// 타국 군주 반응이 나오기 시작할 작위 			
		const int 타국군주_우호도하락량 = 50;   	// 하락하는 우호관계수치

		const int 취임_효과_시작_작위 = 작위_대장군;   	// 취임 이벤트 추가 효과음, 컷인 시작할 작위
		
        const bool 군주가_도시안에_있어야_이벤트발생 = true;        // 군주가 도시안에 있어야만 이벤트 발생, false면 군주가 나가있어도 턴종료시 바로 발생
		
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
        //  즉위 이벤트
        //----------------------------------------------------------------------------------


// 즉위 이벤트

		void callback_peerage(pk::force@ force)
		{
                                  @emperor = pk::get_person(pk::get_scenario().emperor);

		      if (!force.is_player()) return;

            int city_count = 0;
       if (force.title == 작위_없음)	city_count = 2;		    // 작위_없음
       if (force.title == 작위_주자사)	city_count = 4;		// 작위_주자사
       if (force.title == 작위_주목)	city_count = 6;	        // 작위_주목
       if (force.title == 작위_우림중랑장)	city_count = 8;	    // 작위_우림중랑장
       if (force.title == 작위_오관중랑장)	city_count = 12;    // 작위_오관중랑장
       if (force.title == 작위_대장군)	city_count = 14;	// 작위_대장군   

         for (int p = 작위_대장군; p < 작위_끝; p++)
         {			
			if (force.is_player() and force.title == p and pk::get_city_count(force) >= city_count)
			{
				@force = force;
				@kunshu = pk::get_person(force.kunshu);
				@gunshi = pk::get_person(force.gunshi);
				@building = pk::get_building(kunshu.service);

		   // 군주가 도시에 있을 때만
		if (군주가_도시안에_있어야_이벤트발생)
		{		   
            int building_id = building.get_id();
            if (pk::is_absent(kunshu)) return;
            if (pk::is_unitize(kunshu)) return;
            if (kunshu.get_force_id() != building.get_force_id()) return;		
            if (kunshu.location != kunshu.service) return;		
            if (kunshu.mibun == 신분_포로) return;
            if (건물_관문시작 <= building_id and building_id < 건물_항구끝) return;	        
	    }
		
			string current_title_name = pk::decode(pk::get_name(pk::get_title(p)));
			string promotion_title_name = pk::decode(pk::get_name(pk::get_title(p - 1)));	

            int prev_title = force.title;			


		   // 즉위 주동 가신 그룹들

                                 pk::list<pk::person@> senior_list;
                                 senior_list.clear();								 
                                 auto member_list = pk::list_to_array(pk::get_person_list(force, pk::mibun_flags(신분_도독,신분_태수,신분_일반) ) );

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

                                   @senior1 = pk::get_person(무장_문관);
                                   @senior2 = pk::get_person(무장_무관);	
                                   @senior3 = pk::get_person(무장_선신);	
                                   @senior4 = pk::get_person(무장_귀족);	
                                   @senior5 = pk::get_person(무장_부호);	
                                   @senior6 = pk::get_person(무장_학자);
	                   }

                       else if (0 < senior_list.count)
                       {
	                      senior_list.sort(function(a, b)
	                      {				
	                      return float(( a.loyalty * 30 ) / (1 +  (4 * a.rank))) > float(( b.loyalty * 30 ) / (1 + (4 * b.rank)));
				
	                      });

                                   @senior1 = 0 < senior_list.count ? senior_list[0] : pk::get_person(무장_문관);
                                   @senior2 = 1 < senior_list.count ? senior_list[1] : pk::get_person(무장_무관);	
                                   @senior3 = 2 < senior_list.count ? senior_list[2] : pk::get_person(무장_선신);	
                                   @senior4 = 3 < senior_list.count ? senior_list[3] : pk::get_person(무장_귀족);	
                                   @senior5 = 4 < senior_list.count ? senior_list[4] : pk::get_person(무장_부호);	
                                   @senior6 = 5 < senior_list.count ? senior_list[5] : pk::get_person(무장_학자);
	                   }								   



             // 세력 내 한왕실 옹호주의자들
                                 pk::list<pk::person@> loyalist_list;
                                 loyalist_list.clear();									 
                                 auto hansilman_list = pk::list_to_array(pk::get_person_list(force, pk::mibun_flags(신분_도독,신분_태수,신분_일반) ) );

                                 if (0 < hansilman_list.length)
                                  {
           	                      for (int j = 0; j < hansilman_list.length; j++)
           	                      {
	                       pk::person@ loyalist = hansilman_list[j];	
                                     if (pk::is_alive(loyalist) and loyalist.get_id() != kunshu.get_id() and loyalist.kanshitsu == 한실_중시
									 and loyalist.get_id() != senior1.get_id() and loyalist.get_id() != senior2.get_id() and loyalist.get_id() != senior3.get_id()
									 and loyalist.get_id() != senior4.get_id() and loyalist.get_id() != senior5.get_id() and loyalist.get_id() != senior6.get_id()) 	
           	                        {
			// 군주를 친애하거나 부부, 의형제, 부모자식 관계일 경우 제외
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
				
		   // 즉위 소식에 분개할 타국 군주

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




		   // 이벤트 실행 
		   // 황제가 없을때		   
			if (emperor is null)
			{		   
		      pk::scene(pk::scene_t(scene_즉위_천자없음));
			  
               if (0 < loyalist_list.count and 한실주의자_독백보기 and force.title <= 한실주의자_독백_시작_작위)
               {	
		   	    pk::scene(pk::scene_t(scene_한실주의자_독백));
			   }

			   if (force.title == prev_title - 1 and 타국군주_반응보기 and force.title <= 타국군주_반응_시작_작위)
			   {							  
				pk::scene(pk::scene_t(scene_즉위_타국군주반응));					
			   }				   
			}
			
		   // 황제가 아직 있음	   
            else if (emperor !is null)
			{	
			  pk::scene(pk::scene_t(scene_즉위_천자가임명));				   

               if (0 < loyalist_list.count and 한실주의자_독백보기 and force.title <= 한실주의자_독백_시작_작위)
               {	
		   	    pk::scene(pk::scene_t(scene_한실주의자_독백));
			   }

			   if (force.title == prev_title - 1 and 타국군주_반응보기 and force.title <= 타국군주_반응_시작_작위)
			   {							  
				pk::scene(pk::scene_t(scene_즉위_타국군주반응));					
			   }			   
			}


            pk::scene(pk::scene_t(scene_화면_복귀));	
			
			
			if (force.title == prev_title - 1)		
			{			
		   // 히스토리 로그 기록
		      pk::history_log(pk::get_current_turn_force_id(), force.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x(이)가 \x1b[1x{}\x1b[0x의 자리에 오르다" , pk::decode(pk::get_name(kunshu)), promotion_title_name ) ) );
			}

			}

		  } // for

		} // callback_peerage
		


		void scene_즉위_천자없음()
		{
				@force = pk::get_force(kunshu.get_force_id());
			pk::person@ subject1 = pk::is_alive(senior1) ? senior1 : pk::get_person(무장_문관);
			pk::person@ subject2 = pk::is_alive(senior2) ? senior2 : pk::get_person(무장_무관);
			pk::person@ subject3 = pk::is_alive(senior3) ? senior3 : pk::get_person(무장_선신);

			pk::person@ subject4 = pk::is_alive(senior4) ? senior4 : pk::get_person(무장_귀족);
			pk::person@ subject5 = pk::is_alive(senior5) ? senior5 : pk::get_person(무장_부호);
			pk::person@ subject6 = pk::is_alive(senior6) ? senior6 : pk::get_person(무장_학자);

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
			pk::message_box(pk::encode(pk::format("어느 날, \x1b[1x{}\x1b[0x에게 \x1b[2x{}\x1b[0x들이 찾아왔다.", kunshuname, person_names)));
			pk::message_box(pk::encode("주공, 긴히 드릴 말씀이 있습니다..."), subject1);

			pk::council(kunshu, subject1, subject2, subject3, subject4, subject5, subject6, pk::council_t(council_즉위_천자없음_추대));

            int prev_title = force.title;	
			string promotion_title_name = pk::decode(pk::get_name(pk::get_title(prev_title - 1)));			

			pk::background(30);
			pk::fade(255);
			
            pk::play_se(6);

			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 \x1b[1x{}\x1b[0x의 자리에 올랐다.", kunshuname, promotion_title_name)));
			
		   // 작위 변경	
		   
		      force.title = prev_title - 1;
		      force.update();
			  
            if (force.title <= 취임_효과_시작_작위)
            {				
			pk::play_se(10);
			pk::cutin(21);
			pk::play_se(35); 			
	        }			  

            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x 취임을 축하드립니다.", promotion_title_name)), subject1);
			pk::message_box(pk::encode("여러분들이 모두 함께 도와준 덕분이다."), kunshu);
            pk::message_box(pk::encode(pk::format("우리 군의 세력도 성장하고 있고 \x1b[1x{}\x1b[0x 취임이라니 겹경사군요. 축하드립니다.", promotion_title_name)), subject4);			
			pk::message_box(pk::encode("축하드립니다, 주군!"), subject5);			
			pk::message_box(pk::encode("모두들 고맙다. 여러분들이 우리 군을 더욱 더 발전시켜주길 바란다."), kunshu);
			pk::message_box(pk::encode("걱정마십시오. 저희가 주공을 돕겠습니다."), subject6);		
			pk::message_box(pk::encode("우리 군의 번영을 위해 노력하겠습니다!"), subject3);			
            pk::message_box(pk::encode(pk::format("거듭 \x1b[1x{}\x1b[0x 취임 축하드립니다.", promotion_title_name)), subject2);				

			pk::background(38);

                                           string current_year = pk::get_year();	
                                           string current_month = pk::get_month();
                                           string current_day = pk::get_day();			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x년 \x1b[1x{}\x1b[0x월 \x1b[1x{}\x1b[0x일.\n\x1b[1x{}\x1b[0x(은)는 \x1b[1x{}\x1b[0x의 작위를 자칭했습니다.", current_year, current_month, current_day, kunshuname, promotion_title_name )));
					
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		
		} // void scene_즉위_천자없음()



		void council_즉위_천자없음_추대()
		{

			pk::person@ subject1 = pk::is_alive(senior1) ? senior1 : pk::get_person(무장_문관);
			pk::person@ subject2 = pk::is_alive(senior2) ? senior2 : pk::get_person(무장_무관);
			pk::person@ subject3 = pk::is_alive(senior3) ? senior3 : pk::get_person(무장_선신);

			pk::person@ subject4 = pk::is_alive(senior4) ? senior4 : pk::get_person(무장_귀족);
			pk::person@ subject5 = pk::is_alive(senior5) ? senior5 : pk::get_person(무장_부호);
			pk::person@ subject6 = pk::is_alive(senior6) ? senior6 : pk::get_person(무장_학자);

			string kunshuname = pk::decode(pk::get_name(kunshu));
			string subject1name = pk::decode(pk::get_name(subject1));
			
            int prev_title = force.title;	
			string promotion_title_name = pk::decode(pk::get_name(pk::get_title(prev_title - 1)));			
		
			
            pk::message_box(pk::encode(pk::format("주공, 최근 우리 군과 주공의 활약은 이미 전국에 알려져 그 위명을 떨치고 있습니다. 이에 저희 가신들과 신민들은 주공께서 \x1b[1x{}\x1b[0x에 취임해 주시길 바라고 있습니다.", promotion_title_name)), subject1);			
            pk::message_box(pk::encode(pk::format("그렇습니다. 이제 \x1b[1x{}\x1b[0x에 오르실 때 입니다.", promotion_title_name)), subject2);			
			pk::message_box(pk::encode("우리 군의 세력이 커진 만큼, 새로운 작위에 오르셔야 합니다."), subject3);			
            pk::message_box(pk::encode(pk::format("알겠다. 기꺼이 \x1b[1x{}\x1b[0x의 작위에 오르겠다. 이러한 취지를 신민들에게 통지하도록 하라.", promotion_title_name)), kunshu);				



		} // council_즉위_천자없음_추대





		void scene_즉위_천자가임명()
		{
				@force = pk::get_force(kunshu.get_force_id());
			pk::person@ subject1 = pk::is_alive(senior1) ? senior1 : pk::get_person(무장_문관);
			pk::person@ subject2 = pk::is_alive(senior2) ? senior2 : pk::get_person(무장_무관);
			pk::person@ subject3 = pk::is_alive(senior3) ? senior3 : pk::get_person(무장_선신);

			pk::person@ subject4 = pk::is_alive(senior4) ? senior4 : pk::get_person(무장_귀족);
			pk::person@ subject5 = pk::is_alive(senior5) ? senior5 : pk::get_person(무장_부호);
			pk::person@ subject6 = pk::is_alive(senior6) ? senior6 : pk::get_person(무장_학자);

			string kunshuname = pk::decode(pk::get_name(kunshu));
			string subject1name = pk::decode(pk::get_name(subject1));
			string emperorname = pk::decode(pk::get_name(emperor));
						
			pk::move_screen(building.get_pos());
            pk::play_bgm(3);
			pk::message_box(pk::encode(pk::format("어느 날, \x1b[1x{}\x1b[0x에게 황제의 사자가 찾아왔다.", kunshuname)));
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x님, \x1b[1x{}\x1b[0x님으로부터 칙사가 왔습니다. 작위를 수여한다는 소식입니다.", kunshuname, emperorname)), pk::get_person(무장_문관));

			pk::person@ aide = pk::is_alive(gunshi) ? gunshi : subject1;

			pk::diplomacy(kunshu, aide, pk::get_person(무장_사자), pk::diplomacy_t(diplomacy_즉위_천자의_사자와_독대));



			pk::background(30);
			pk::fade(255);	
			
            pk::play_se(6);

            int prev_title = force.title;	
			string promotion_title_name = pk::decode(pk::get_name(pk::get_title(prev_title - 1)));			
		
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 \x1b[1x{}\x1b[0x의 자리에 올랐다.", kunshuname, promotion_title_name)));
				
		   // 작위 변경
		   
		      force.title = prev_title - 1;
		      force.update();
			  
            if (force.title <= 취임_효과_시작_작위)
            {	                      
			pk::play_se(10);
			pk::cutin(21);
			pk::play_se(35); 			
	        }			  

            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x 취임을 축하드립니다.", promotion_title_name)), subject1);
			pk::message_box(pk::encode("여러분들이 모두 함께 도와준 덕분이다."), kunshu);
            pk::message_box(pk::encode(pk::format("우리 군의 세력도 성장하고 있고 \x1b[1x{}\x1b[0x 취임이라니 겹경사군요. 축하드립니다.", promotion_title_name)), subject4);			
			pk::message_box(pk::encode("축하드립니다, 주군!"), subject5);			
			pk::message_box(pk::encode("모두들 고맙다. 여러분들이 우리 군을 더욱 더 발전시켜주길 바란다."), kunshu);
			pk::message_box(pk::encode("걱정마십시오. 저희가 주공을 돕겠습니다."), subject6);		
			pk::message_box(pk::encode("우리 군의 번영을 위해 노력하겠습니다!"), subject3);			
            pk::message_box(pk::encode(pk::format("거듭 \x1b[1x{}\x1b[0x 취임 축하드립니다.", promotion_title_name)), subject2);				

			pk::background(34);
			
                                           string current_year = pk::get_year();	
                                           string current_month = pk::get_month();
                                           string current_day = pk::get_day();			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x년 \x1b[1x{}\x1b[0x월 \x1b[1x{}\x1b[0x일.\n\x1b[1x{}\x1b[0x(은)는 \x1b[1x{}\x1b[0x로부터 \x1b[1x{}\x1b[0x의 작위를 받았습니다.", current_year, current_month, current_day, kunshuname, emperorname, promotion_title_name )));
					
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		
		} // void scene_즉위_천자가임명()



		void diplomacy_즉위_천자의_사자와_독대()
		{

			string kunshuname = pk::decode(pk::get_name(kunshu));
            int prev_title = force.title;	
			string promotion_title_name = pk::decode(pk::get_name(pk::get_title(prev_title - 1)));			
   
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x, 그대를 \x1b[1x{}\x1b[0x에 임명한다. 앞으로도 \x1b[1x한\x1b[0x조를 위해 진력하라.", kunshuname, promotion_title_name )), pk::get_person(무장_사자));
			pk::message_box(pk::encode("감사합니다. 기대에 어긋나지 않도록 더 열심히 하겠습니다."), kunshu);


		} // diplomacy_즉위_천자의_사자와_독대()



		void scene_한실주의자_독백()
		{
				@force = pk::get_force(kunshu.get_force_id());

			string kunshuname = pk::decode(pk::get_name(kunshu));
	
			string current_title = pk::decode(pk::get_name(pk::get_title(force.title)));		
   	
			
               if (kunshu.kanshitsu == 한실_중시)
			   {
			   pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x의 \x1b[1x{}\x1b[0x 취임 소식에 한실을 중시하는 일부 가신들의 반응은...", kunshuname, current_title)));	
               pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x님은 분명 한실의 신하로서 충성을 다 하실 것이다.", kunshuname )), maverick1);		   
				  
			   }

               else if (kunshu.kanshitsu != 한실_중시)
			   {
			   pk::message_box(pk::encode(pk::format("하지만, \x1b[1x{}\x1b[0x의 \x1b[1x{}\x1b[0x 취임 소식에 한실을 중시하는 일부 가신들의 반응은...", kunshuname, current_title)));	
               pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x님이 \x1b[1x{}\x1b[0x에 임명되신 것은 기쁜 일이지만... 사람이 높은 지위에 오를 수록 욕심도 더 크게 늘어나는 것은 자연스런 이치다...한실의 신하로서의 자각을 잃지 않으셔야 할터인데...마냥 좋아할 수만은 없구나...", kunshuname, current_title )), maverick1);
			   }
			   
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		
		} // void scene_한실주의자_독백()






        //----------------------------------------------------------------------------------
        //  즉위 타국 반응
        //----------------------------------------------------------------------------------



		void scene_즉위_타국군주반응()
		{
			string kunshuname = pk::decode(pk::get_name(kunshu));
		   // 즉위 소식에 분개할 타국 군주 대사
            if (pk::is_alive(enemy_kunshu) and pk::is_alive(enemy_force_1)) 	
		    {
             if (enemy_kunshu.get_id() == kunshu.get_id()) 
                 return;
				
			pk::move_screen(enemy_building.get_pos());
            pk::play_bgm(20);
			
			string current_title = pk::decode(pk::get_name(pk::get_title(force.title)));			
			pk::message_box(pk::encode(pk::format("한편, \x1b[1x{}\x1b[0x의 \x1b[1x{}\x1b[0x 취임 소식은 다른 군주들에겐 경계심을 더욱 키우는 계기가 되었다.", kunshuname, current_title)));
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x의 야심이 점점 더 커지겠구나... 방심해선 안될 것이다." , kunshuname)), enemy_kunshu);

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
               pk::add_relations(enemy_force_group, force.get_id(), -타국군주_우호도하락량 );				
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

            pk::message_box(pk::encode(pk::format("\x1b[2x{}\x1b[0x - \x1b[2x{}\x1b[0x명의 군주(들)의 \x1b[1x{}\x1b[0x에 대한 우호도가 크게 하락했다.", person_names, enemy_force_group_list.count, kunshuname)));
 
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);	
		    }
	
		} // scene_즉위_타국군주반응()





        //----------------------------------------------------------------------------------
        //  기타
        //----------------------------------------------------------------------------------




// 화면 복귀
		void scene_화면_복귀()
		{
			pk::move_screen(building.get_pos());		
		} // scene_화면_복귀()
	
	

// 친분있는 관계 여부

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
		

// 공동의 친분있는 지인이 있는지 여부
	 int func_mutual_relation_check(pk::person@ person, pk::person@ suspect)
	 {
		 int relationcheck = 0;
		 pk::list<pk::person@> friend_list;
         friend_list.clear();		 
	   for (int i = 0; i < 무장_끝; i++)
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