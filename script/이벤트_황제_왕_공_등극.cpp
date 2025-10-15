namespace 이벤트_황제_왕_공_등극
{
	
	// ================ CUSTOMIZE ================

		const int duke_city_count = 16;   	// 공 이벤트 조건 16개 도시		
		const int king_city_count = 20;   	// 왕 이벤트 조건 20개 도시
		const int emperor_city_count = 24;   	// 황제 이벤트 조건 24개 도시

        const bool 황제협박_매력감소 = true;        // true=적용, false=미적용, 황제협박시 매력 감소 여부
		const int 협박_매력_감소량 = 10;   	// 황제협박시 매력 감소량		

        const bool 한조중시가신_충성감소 = true;        // 국호가 한이 아닐때 생기는 한조중시가신들의 충성 감소 여부
		const int 국호변경_충성감소 = 30;   	// 국호가 한이 아닐때 생기는 한조 중시 가신들의 충성 감소량		
		const int 국호변경_충성감소_양위 = 3;   	// 국호가 한이 아니긴 하지만, 양위라 적게 감소하는 한조 중시 가신들의 충성 감소량
		const int 폐위_충성감소 = 60;   	// 황제 폐위에 충격먹은 한조 중시 가신들의 충성 감소량

        const bool 한조중시가신_충성증가 = true;        // 국호가 한으로 유지 되어 생기는 한조중시가신들의 충성 증가 여부
		const int 국호유지_충성증가 = 10;   	// 국호가 한으로 유지될때 한조 중시 가신들의 충성 증가량
		
        const bool 타국군주_반응보기 = true;        // 황제 즉위에 분노하는 타국 군주 반응 발생 여부
		const int 타국군주_우호도하락량 = 50;   	// 황제 즉위에 분노하여 하락하는 우호관계수치

        const bool 황제_처형 = true;        // 황제 처형에 대한 고려 여부
		const int 폐위와처형_충성감소 = 100;   	// 황제 처형에 분노하여 하락하는 충성도
		const int 타국군주_우호도하락량_황제처형 = 100;   	// 황제 처형에 분노하여 하락하는 우호관계수치	

        const bool 황제처형_매력감소 = true;        // 황제처형시 매력 감소 여부
		const int 처형_매력_감소량 = 20;   	// 황제처형시 매력 감소량

        const bool 강제폐위로_플레이어_혐오 = true;        // 황제 폐위로 인해 한실주의자들이 플레이어를 혐오하기 시작

        const bool 군주가_도시안에_있어야_이벤트발생 = true;        // 군주가 도시안에 있어야만 이벤트 발생, false면 군주가 나가있어도 턴종료시 바로 발생

		const int 양위_조건 = 0;   	// 0: 군주가 한실중시 + 황제의 친애장수여야함,  1: 군주가 한실중시이기만 하면 됨
		
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
        //  공 즉위 이벤트
        //----------------------------------------------------------------------------------


// 공 즉위 이벤트

		void callback_duke(pk::force@ force)
		{
                                  @emperor = pk::get_person(pk::get_scenario().emperor);

		      if (!force.is_player()) return;
		      if (force.title != 작위_대사마) return;
		      if (pk::get_city_count(force) < duke_city_count ) return;

			
			if (force.is_player() and force.title == 작위_대사마 and pk::get_city_count(force) >= duke_city_count)
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



           // 황제 강제 폐위 추진 세력
                                 pk::list<pk::person@> pro_dethrone_list;
                                 pro_dethrone_list.clear();								 
                                 auto radical_list = pk::list_to_array(pk::get_person_list(force, pk::mibun_flags(신분_도독,신분_태수,신분_일반) ) );

                                 if (0 < radical_list.length)
                                  {
           	                      for (int k = 0; k < radical_list.length; k++)
           	                      {
	                       pk::person@ radical = radical_list[k];	
                                     if (pk::is_alive(radical) and radical.get_id() != kunshu.get_id() and radical.kanshitsu == 한실_무시) 	
                                     pro_dethrone_list.add(radical);

	                       }

                                 }


                       if (0 == pro_dethrone_list.count)
                       {

                                   @pro_dethrone1 = pk::get_person(무장_문관);
                                   @pro_dethrone2 = pk::get_person(무장_무관);	
                                   @pro_dethrone3 = pk::get_person(무장_선신);	
                                   @pro_dethrone4 = pk::get_person(무장_귀족);	
                                   @pro_dethrone5 = pk::get_person(무장_부호);	
                                   @pro_dethrone6 = pk::get_person(무장_학자);
								   
	                   }

                       else if (0 < pro_dethrone_list.count)
                       {
	                      pro_dethrone_list.sort(function(a, b)
	                      {				
	                      return float(( a.loyalty * 30 ) / (1 +  (4 * a.rank))) > float(( b.loyalty * 30 ) / (1 + (4 * b.rank)));
 
	                      });

                                   @pro_dethrone1 = 0 < pro_dethrone_list.count ? pro_dethrone_list[0] : pk::get_person(무장_문관);
                                   @pro_dethrone2 = 1 < pro_dethrone_list.count ? pro_dethrone_list[1] : pk::get_person(무장_무관);	
                                   @pro_dethrone3 = 2 < pro_dethrone_list.count ? pro_dethrone_list[2] : pk::get_person(무장_선신);	
                                   @pro_dethrone4 = 3 < pro_dethrone_list.count ? pro_dethrone_list[3] : pk::get_person(무장_귀족);	
                                   @pro_dethrone5 = 4 < pro_dethrone_list.count ? pro_dethrone_list[4] : pk::get_person(무장_부호);	
                                   @pro_dethrone6 = 5 < pro_dethrone_list.count ? pro_dethrone_list[5] : pk::get_person(무장_학자);

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


		   // 즉위 소식에 분개할 한실 중시 군주

		    pk::list<pk::force@> pro_han_force_list;
            pro_han_force_list.clear();			
			for (int i = 0; i < force_list.count; i++)
			{
	        pk::force@ pro_han_force = force_list[i];
	        pk::person@ pro_han_kunshu_group = pk::get_person(pk::get_kunshu_id(pro_han_force));
            if (pro_han_force.get_id() != force.get_id() and pro_han_kunshu_group.kanshitsu == 한실_중시) 	
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

		   // 이벤트 실행 
		   // 황제가 없을때		   
			if (emperor is null)
			{		   
		      pk::scene(pk::scene_t(scene_공즉위_천자없음));
			  
               if (0 < loyalist_list.count)
               {	
		   	    pk::scene(pk::scene_t(scene_한실주의자_독백_공));
			   }

			   if (force.title == 작위_공 and 타국군주_반응보기)
			   {							  
				pk::scene(pk::scene_t(scene_왕공즉위_타국군주반응));					
			   }				   
			}
			
		   // 황제가 아직 있음	   
            else if (emperor !is null)
			{	
			  pk::scene(pk::scene_t(scene_공즉위_천자가임명));				   

               if (0 < loyalist_list.count)
               {	
		   	    pk::scene(pk::scene_t(scene_한실주의자_독백_공));
			   }

			   if (force.title == 작위_공 and 타국군주_반응보기)
			   {							  
				pk::scene(pk::scene_t(scene_왕공즉위_타국군주반응));					
			   }			   
			}


            pk::scene(pk::scene_t(scene_화면_복귀));	
			
			
			if (force.title == 작위_공)		
			{			
		   // 히스토리 로그 기록
			  string duke_title = pk::decode(pk::get_name(pk::get_title(작위_공)));		   
		      pk::history_log(pk::get_current_turn_force_id(), force.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x(이)가 \x1b[1x{}\x1b[0x의 자리에 오르다" , pk::decode(pk::get_name(kunshu)), duke_title ) ) );
			}

			}


		} // callback_duke
		


		void scene_공즉위_천자없음()
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

			pk::council(kunshu, subject1, subject2, subject3, subject4, subject5, subject6, pk::council_t(council_공즉위_천자없음_추대));



			pk::background(30);
			pk::fade(255);
			
            pk::play_se(6);

			string duke_title = pk::decode(pk::get_name(pk::get_title(작위_공)));
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 마침내 \x1b[1x{}\x1b[0x의 자리에 올랐다.", kunshuname, duke_title)));
			pk::play_se(10);
			pk::cutin(21);
			pk::play_se(35); 			


		   // 작위 변경

		      force.title = 작위_공;
		      force.update();

            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x 즉위를 경하드립니다.", duke_title)), subject1);
			pk::message_box(pk::encode("여러분이 모두 함께 고생해준 덕분이다."), kunshu);
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x 작위의 이름을 걸고, 앞으로도 더욱 정진하겠습니다.", duke_title)), subject4);			
			pk::message_box(pk::encode("저희들도 한층 더 노력해야겠습니다."), subject5);			
			pk::message_box(pk::encode("음, 다들 앞으로도 잘 부탁한다. 그대들도 앞으로 잘 부탁한다."), kunshu);
            pk::message_box(pk::encode(pk::format("염려놓으십시오. \x1b[1x{}\x1b[0x의 신하라는 이름에 걸맞게 최선을 다하겠습니다.", duke_title)), subject6);			
			pk::message_box(pk::encode("더욱 분골쇄신하겠습니다."), subject3);			
			pk::message_box(pk::encode("다시 한번 경하드립니다."), subject2);

			pk::background(38);

                                           string current_year = pk::get_year();	
                                           string current_month = pk::get_month();
                                           string current_day = pk::get_day();			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x년 \x1b[1x{}\x1b[0x월 \x1b[1x{}\x1b[0x일.\n\x1b[1x{}\x1b[0x(은)는 \x1b[1x{}\x1b[0x의 작위를 자칭했습니다.", current_year, current_month, current_day, kunshuname, duke_title )));
					
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		
		} // void scene_공즉위_천자없음()



		void council_공즉위_천자없음_추대()
		{

			pk::person@ subject1 = pk::is_alive(senior1) ? senior1 : pk::get_person(무장_문관);
			pk::person@ subject2 = pk::is_alive(senior2) ? senior2 : pk::get_person(무장_무관);
			pk::person@ subject3 = pk::is_alive(senior3) ? senior3 : pk::get_person(무장_선신);

			pk::person@ subject4 = pk::is_alive(senior4) ? senior4 : pk::get_person(무장_귀족);
			pk::person@ subject5 = pk::is_alive(senior5) ? senior5 : pk::get_person(무장_부호);
			pk::person@ subject6 = pk::is_alive(senior6) ? senior6 : pk::get_person(무장_학자);

			string kunshuname = pk::decode(pk::get_name(kunshu));
			string subject1name = pk::decode(pk::get_name(subject1));
			string duke_title = pk::decode(pk::get_name(pk::get_title(작위_공)));			
			
            pk::message_box(pk::encode(pk::format("주공, 주공의 공적은 사해에 알려져 그 위세가 천하를 뒤덮고 있습니다. 부디 저희 신민의 뜻을 잘 헤아리셔서 \x1b[1x{}\x1b[0x에 취임해 주십시오.", duke_title)), subject1);			
            pk::message_box(pk::encode(pk::format("그렇습니다. 이제 \x1b[1x{}\x1b[0x에 오르실 때 입니다.", duke_title)), subject2);			
			pk::message_box(pk::encode("이는 시의적절합니다. 주공의 명성에 걸맞는 새로운 작위에 오르셔야 합니다."), subject3);			
            pk::message_box(pk::encode(pk::format("여러분의 기대에 보답하기 위해서 지금부터 \x1b[1x{}\x1b[0x의 작위에 오르겠노라. 이러한 취지를 신민들에게 통지하라.", duke_title)), kunshu);				



		} // council_공즉위_천자없음_추대





		void scene_공즉위_천자가임명()
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
			string duke_title = pk::decode(pk::get_name(pk::get_title(작위_공)));
			string king_title = pk::decode(pk::get_name(pk::get_title(작위_왕)));			
			
			pk::move_screen(building.get_pos());
            pk::play_bgm(3);
			pk::message_box(pk::encode(pk::format("어느 날, \x1b[1x{}\x1b[0x에게 황제의 사자가 찾아왔다.", kunshuname)));
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x님, \x1b[1x{}\x1b[0x님으로부터 \x1b[1x{}\x1b[0x\x1b[1x{}\x1b[0x 서임의 특사가 왔습니다.", kunshuname, emperorname, king_title, duke_title)), pk::get_person(무장_사자));

			pk::diplomacy(emperor, null, kunshu, pk::diplomacy_t(diplomacy_공즉위_천자와독대));



			pk::background(30);
			pk::fade(255);	
			
            pk::play_se(6);

			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 마침내 \x1b[1x{}\x1b[0x의 자리에 올랐다.", kunshuname, duke_title)));
			pk::play_se(10);
			pk::cutin(21);
			pk::play_se(35); 			

		   // 작위 변경

		      force.title = 작위_공;
		      force.update();

            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x 즉위를 경하드립니다.", duke_title)), subject1);
			pk::message_box(pk::encode("여러분이 모두 함께 고생해준 덕분이다."), kunshu);
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x 작위의 이름을 걸고, 앞으로도 더욱 정진하겠습니다.", duke_title)), subject4);			
			pk::message_box(pk::encode("저희들도 한층 더 노력해야겠습니다."), subject5);			
			pk::message_box(pk::encode("음, 다들 앞으로도 잘 부탁한다. 그대들도 앞으로 잘 부탁한다."), kunshu);
            pk::message_box(pk::encode(pk::format("염려놓으십시오. \x1b[1x{}\x1b[0x의 신하라는 이름에 걸맞게 최선을 다하겠습니다.", duke_title)), subject6);			
			pk::message_box(pk::encode("더욱 분골쇄신하겠습니다."), subject3);			
			pk::message_box(pk::encode("다시 한번 경하드립니다."), subject2);


			pk::background(34);
			
                                           string current_year = pk::get_year();	
                                           string current_month = pk::get_month();
                                           string current_day = pk::get_day();			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x년 \x1b[1x{}\x1b[0x월 \x1b[1x{}\x1b[0x일.\n\x1b[1x{}\x1b[0x(은)는 \x1b[1x{}\x1b[0x로부터 \x1b[1x{}\x1b[0x의 작위에 봉해졌습니다.", current_year, current_month, current_day, kunshuname, emperorname, duke_title )));
					
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		
		} // void scene_공즉위_천자가임명()



		void diplomacy_공즉위_천자와독대()
		{

			string kunshuname = pk::decode(pk::get_name(kunshu));
			string duke_title = pk::decode(pk::get_name(pk::get_title(작위_공)));			
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x, 그대를 \x1b[1x{}\x1b[0x에 봉하노라. 앞으로도 \x1b[1x한\x1b[0x조를 위해 진력하라.", kunshuname, duke_title )), emperor);
			pk::message_box(pk::encode("감사합니다. 폐하의 기대에 보답할 수 있도록 앞으로도 열심히 하겠습니다."), kunshu);


		} // diplomacy_공즉위_천자와독대()



		void scene_한실주의자_독백_공()
		{
				@force = pk::get_force(kunshu.get_force_id());

			string kunshuname = pk::decode(pk::get_name(kunshu));
			string duke_title = pk::decode(pk::get_name(pk::get_title(작위_공)));	
			
               if (kunshu.kanshitsu == 한실_중시)
			   {
			   pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x의 \x1b[1x{}\x1b[0x 즉위 소식에 한실을 중시하는 일부 가신들의 반응은...", kunshuname, duke_title )));	
               pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x님은 분명 한실의 신하로서 충성을 다 하실 것이다.", kunshuname )), maverick1);		   
				  
			   }

               else if (kunshu.kanshitsu != 한실_중시)
			   {
			   pk::message_box(pk::encode(pk::format("하지만, \x1b[1x{}\x1b[0x의 \x1b[1x{}\x1b[0x 즉위 소식에 한실을 중시하는 일부 가신들의 반응은...", kunshuname, duke_title )));	
               pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x님이 \x1b[1x{}\x1b[0x에 즉위하신 것은 기쁜 일이지만... 사람이 높은 지위에 오를 수록 욕심도 더 크게 늘어나는 것은 자연스런 이치다...한실의 신하로서의 자각을 잃지 않으셔야 할터인데...마냥 좋아할 수만은 없구나...", kunshuname, duke_title )), maverick1);
			   }
			   
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		
		} // void scene_한실주의자_독백_공()




        //----------------------------------------------------------------------------------
        //  왕 즉위 이벤트
        //----------------------------------------------------------------------------------


// 왕 즉위 이벤트

		void callback_king(pk::force@ force)
		{
                                  @emperor = pk::get_person(pk::get_scenario().emperor);

		      if (!force.is_player()) return;
		      if (force.title != 작위_공) return;
		      if (pk::get_city_count(force) < king_city_count ) return;

			
			if (force.is_player() and force.title == 작위_공 and pk::get_city_count(force) >= king_city_count)
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



           // 황제 강제 폐위 추진 세력
                                 pk::list<pk::person@> pro_dethrone_list;
                                 pro_dethrone_list.clear();								 
                                 auto radical_list = pk::list_to_array(pk::get_person_list(force, pk::mibun_flags(신분_도독,신분_태수,신분_일반) ) );

                                 if (0 < radical_list.length)
                                  {
           	                      for (int k = 0; k < radical_list.length; k++)
           	                      {
	                       pk::person@ radical = radical_list[k];	
                                     if (pk::is_alive(radical) and radical.get_id() != kunshu.get_id() and radical.kanshitsu == 한실_무시) 	
                                     pro_dethrone_list.add(radical);

	                       }

                                 }

                       if (0 == pro_dethrone_list.count)
                       {

                                   @pro_dethrone1 = pk::get_person(무장_문관);
                                   @pro_dethrone2 = pk::get_person(무장_무관);	
                                   @pro_dethrone3 = pk::get_person(무장_선신);	
                                   @pro_dethrone4 = pk::get_person(무장_귀족);	
                                   @pro_dethrone5 = pk::get_person(무장_부호);	
                                   @pro_dethrone6 = pk::get_person(무장_학자);
								   
	                   }

                       else if (0 < pro_dethrone_list.count)
                       {
	                      pro_dethrone_list.sort(function(a, b)
	                      {				
	                      return float(( a.loyalty * 30 ) / (1 +  (4 * a.rank))) > float(( b.loyalty * 30 ) / (1 + (4 * b.rank)));
 
	                      });

                                   @pro_dethrone1 = 0 < pro_dethrone_list.count ? pro_dethrone_list[0] : pk::get_person(무장_문관);
                                   @pro_dethrone2 = 1 < pro_dethrone_list.count ? pro_dethrone_list[1] : pk::get_person(무장_무관);	
                                   @pro_dethrone3 = 2 < pro_dethrone_list.count ? pro_dethrone_list[2] : pk::get_person(무장_선신);	
                                   @pro_dethrone4 = 3 < pro_dethrone_list.count ? pro_dethrone_list[3] : pk::get_person(무장_귀족);	
                                   @pro_dethrone5 = 4 < pro_dethrone_list.count ? pro_dethrone_list[4] : pk::get_person(무장_부호);	
                                   @pro_dethrone6 = 5 < pro_dethrone_list.count ? pro_dethrone_list[5] : pk::get_person(무장_학자);

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


		   // 즉위 소식에 분개할 한실 중시 군주

		    pk::list<pk::force@> pro_han_force_list;
            pro_han_force_list.clear();			
			for (int i = 0; i < force_list.count; i++)
			{
	        pk::force@ pro_han_force = force_list[i];
	        pk::person@ pro_han_kunshu_group = pk::get_person(pk::get_kunshu_id(pro_han_force));
            if (pro_han_force.get_id() != force.get_id() and pro_han_kunshu_group.kanshitsu == 한실_중시) 	
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

		   // 이벤트 실행 
		   // 황제가 없을때		   
			if (emperor is null)
			{		   
		      pk::scene(pk::scene_t(scene_왕즉위_천자없음));
			  
               if (0 < loyalist_list.count)
               {	
		   	    pk::scene(pk::scene_t(scene_한실주의자_독백));
			   }

			   if (force.title == 작위_왕 and 타국군주_반응보기)
			   {							  
				pk::scene(pk::scene_t(scene_왕공즉위_타국군주반응));					
			   }				   
			}
			
		   // 황제가 아직 있음	   
            else if (emperor !is null)
			{	
			  pk::scene(pk::scene_t(scene_왕즉위_천자가임명));				   

               if (0 < loyalist_list.count)
               {	
		   	    pk::scene(pk::scene_t(scene_한실주의자_독백));
			   }

			   if (force.title == 작위_왕 and 타국군주_반응보기)
			   {							  
				pk::scene(pk::scene_t(scene_왕공즉위_타국군주반응));					
			   }			   
			}


            pk::scene(pk::scene_t(scene_화면_복귀));	
			
			
			if (force.title == 작위_왕)		
			{			
		   // 히스토리 로그 기록
			string king_title = pk::decode(pk::get_name(pk::get_title(작위_왕)));		   
		      pk::history_log(pk::get_current_turn_force_id(), force.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x(이)가 \x1b[1x{}\x1b[0x의 자리에 오르다" , pk::decode(pk::get_name(kunshu)), king_title ) ) );
			}

			}


		} // callback_king
		


		void scene_왕즉위_천자없음()
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

			pk::council(kunshu, subject1, subject2, subject3, subject4, subject5, subject6, pk::council_t(council_왕즉위_천자없음_추대));



			pk::background(30);
			pk::fade(255);
			
            pk::play_se(6);

			string king_title = pk::decode(pk::get_name(pk::get_title(작위_왕)));
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 마침내 \x1b[1x{}\x1b[0x의 자리에 올랐다.", kunshuname, king_title)));
			pk::play_se(10);
			pk::cutin(21);
			pk::play_se(35); 			


		   // 작위 변경

		      force.title = 작위_왕;
		      force.update();

            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x 즉위를 경하드립니다.", king_title)), subject1);
			pk::message_box(pk::encode("여러분이 모두 함께 고생해준 덕분이다."), kunshu);
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x 작위의 이름을 걸고, 앞으로도 더욱 정진하겠습니다.", king_title)), subject4);			
			pk::message_box(pk::encode("저희들도 한층 더 노력해야겠습니다."), subject5);			
			pk::message_box(pk::encode("음, 다들 앞으로도 잘 부탁한다. 그대들도 앞으로 잘 부탁한다."), kunshu);
            pk::message_box(pk::encode(pk::format("염려놓으십시오. \x1b[1x{}\x1b[0x의 신하라는 이름에 걸맞게 최선을 다하겠습니다.", king_title)), subject6);			
			pk::message_box(pk::encode("더욱 분골쇄신하겠습니다."), subject3);			
			pk::message_box(pk::encode("다시 한번 경하드립니다."), subject2);

			pk::background(38);

                                           string current_year = pk::get_year();	
                                           string current_month = pk::get_month();
                                           string current_day = pk::get_day();			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x년 \x1b[1x{}\x1b[0x월 \x1b[1x{}\x1b[0x일.\n\x1b[1x{}\x1b[0x(은)는 \x1b[1x{}\x1b[0x의 작위를 자칭했습니다.", current_year, current_month, current_day, kunshuname, king_title )));
					
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		
		} // void scene_왕즉위_천자없음()



		void council_왕즉위_천자없음_추대()
		{

			pk::person@ subject1 = pk::is_alive(senior1) ? senior1 : pk::get_person(무장_문관);
			pk::person@ subject2 = pk::is_alive(senior2) ? senior2 : pk::get_person(무장_무관);
			pk::person@ subject3 = pk::is_alive(senior3) ? senior3 : pk::get_person(무장_선신);

			pk::person@ subject4 = pk::is_alive(senior4) ? senior4 : pk::get_person(무장_귀족);
			pk::person@ subject5 = pk::is_alive(senior5) ? senior5 : pk::get_person(무장_부호);
			pk::person@ subject6 = pk::is_alive(senior6) ? senior6 : pk::get_person(무장_학자);

			string kunshuname = pk::decode(pk::get_name(kunshu));
			string subject1name = pk::decode(pk::get_name(subject1));
			string king_title = pk::decode(pk::get_name(pk::get_title(작위_왕)));
			
            pk::message_box(pk::encode(pk::format("주공, 주공의 공적은 사해에 알려져 그 위세가 천하를 뒤덮고 있습니다. 부디 저희 신민의 뜻을 잘 헤아리셔서 \x1b[1x{}\x1b[0x에 취임해 주십시오.", king_title)), subject1);			
            pk::message_box(pk::encode(pk::format("그렇습니다. 이제 \x1b[1x{}\x1b[0x에 오르실 때 입니다.", king_title)), subject2);			
			pk::message_box(pk::encode("이는 시의적절합니다. 주공의 명성에 걸맞는 새로운 작위에 오르셔야 합니다."), subject3);			
            pk::message_box(pk::encode(pk::format("여러분의 기대에 보답하기 위해서 지금부터 \x1b[1x{}\x1b[0x의 작위에 오르겠노라. 이러한 취지를 신민들에게 통지하라.", king_title)), kunshu);			
			
		} // council_왕즉위_천자없음_추대





		void scene_왕즉위_천자가임명()
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
			string duke_title = pk::decode(pk::get_name(pk::get_title(작위_공)));			
			string king_title = pk::decode(pk::get_name(pk::get_title(작위_왕)));			
	
			
			pk::move_screen(building.get_pos());
            pk::play_bgm(3);
			pk::message_box(pk::encode(pk::format("어느 날, \x1b[1x{}\x1b[0x에게 황제의 사자가 찾아왔다.", kunshuname)));
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x님, \x1b[1x{}\x1b[0x님으로부터 \x1b[1x{}\x1b[0x\x1b[1x{}\x1b[0x 서임의 특사가 왔습니다.", kunshuname, emperorname, king_title, duke_title)), pk::get_person(무장_사자));
			
			pk::diplomacy(emperor, null, kunshu, pk::diplomacy_t(diplomacy_왕즉위_천자와독대));



			pk::background(30);
			pk::fade(255);	
			
            pk::play_se(6);

			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 마침내 \x1b[1x{}\x1b[0x의 자리에 올랐다.", kunshuname, king_title)));
			pk::play_se(10);
			pk::cutin(21);
			pk::play_se(35); 			

		   // 작위 변경

		      force.title = 작위_왕;
		      force.update();

            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x 즉위를 경하드립니다.", king_title)), subject1);
			pk::message_box(pk::encode("여러분이 모두 함께 고생해준 덕분이다."), kunshu);
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x 작위의 이름을 걸고, 앞으로도 더욱 정진하겠습니다.", king_title)), subject4);			
			pk::message_box(pk::encode("저희들도 한층 더 노력해야겠습니다."), subject5);			
			pk::message_box(pk::encode("음, 다들 앞으로도 잘 부탁한다. 그대들도 앞으로 잘 부탁한다."), kunshu);
            pk::message_box(pk::encode(pk::format("염려놓으십시오. \x1b[1x{}\x1b[0x의 신하라는 이름에 걸맞게 최선을 다하겠습니다.", king_title)), subject6);			
			pk::message_box(pk::encode("더욱 분골쇄신하겠습니다."), subject3);			
			pk::message_box(pk::encode("다시 한번 경하드립니다."), subject2);


			pk::background(34);
			
                                           string current_year = pk::get_year();	
                                           string current_month = pk::get_month();
                                           string current_day = pk::get_day();			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x년 \x1b[1x{}\x1b[0x월 \x1b[1x{}\x1b[0x일.\n\x1b[1x{}\x1b[0x(은)는 \x1b[1x{}\x1b[0x로부터 \n\x1b[1x{}\x1b[0x의 작위에 봉해졌습니다.", current_year, current_month, current_day, kunshuname, emperorname, king_title )));
					
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		
		} // void scene_왕즉위_천자가임명()



		void diplomacy_왕즉위_천자와독대()
		{

			string kunshuname = pk::decode(pk::get_name(kunshu));
			string king_title = pk::decode(pk::get_name(pk::get_title(작위_왕)));
			
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x, 그대를 \x1b[1x{}\x1b[0x에 봉하노라. 앞으로도 \x1b[1x한\x1b[0x조를 위해 진력하라.", kunshuname, king_title )), emperor);
			pk::message_box(pk::encode("감사합니다. 폐하의 기대에 보답할 수 있도록 앞으로도 열심히 하겠습니다."), kunshu);


		} // diplomacy_왕즉위_천자와독대()



		void scene_한실주의자_독백()
		{
				@force = pk::get_force(kunshu.get_force_id());

			string kunshuname = pk::decode(pk::get_name(kunshu));
			string king_title = pk::decode(pk::get_name(pk::get_title(작위_왕)));
			
               if (kunshu.kanshitsu == 한실_중시)
			   {
			   pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x의 \x1b[1x{}\x1b[0x 즉위 소식에 한실을 중시하는 일부 가신들의 반응은...", kunshuname, king_title )));	
               pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x님은 분명 한실의 신하로서 충성을 다 하실 것이다.", kunshuname )), maverick1);		   
				  
			   }

               else if (kunshu.kanshitsu != 한실_중시)
			   {
			   pk::message_box(pk::encode(pk::format("하지만, \x1b[1x{}\x1b[0x의 \x1b[1x{}\x1b[0x 즉위 소식에 한실을 중시하는 일부 가신들의 반응은...", kunshuname, king_title )));	
               pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x님이 \x1b[1x{}\x1b[0x에 즉위하신 것은 기쁜 일이지만... 사람이 높은 지위에 오를 수록 욕심도 더 크게 늘어나는 것은 자연스런 이치다...한실의 신하로서의 자각을 잃지 않으셔야 할터인데...마냥 좋아할 수만은 없구나...", kunshuname, king_title )), maverick1);
			   }
			   
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		
		} // void scene_한실주의자_독백()




        //----------------------------------------------------------------------------------
        //  황제 즉위 이벤트
        //----------------------------------------------------------------------------------




// 황제 즉위 이벤트


		void callback_emperor(pk::force@ force)
		{
                                  @emperor = pk::get_person(pk::get_scenario().emperor);

		      if (!force.is_player()) return;
		      if (force.title != 작위_왕) return;
		      if (pk::get_city_count(force) < emperor_city_count ) return;

			
		  if (force.is_player() and force.title == 작위_왕 and pk::get_city_count(force) >= emperor_city_count)
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


           // 황제 강제 폐위 추진 세력
                                 pk::list<pk::person@> pro_dethrone_list;
                                 pro_dethrone_list.clear();								 
                                 auto radical_list = pk::list_to_array(pk::get_person_list(force, pk::mibun_flags(신분_도독,신분_태수,신분_일반) ) );

                                 if (0 < radical_list.length)
                                  {
           	                      for (int k = 0; k < radical_list.length; k++)
           	                      {
	                       pk::person@ radical = radical_list[k];	
                                     if (pk::is_alive(radical) and radical.get_id() != kunshu.get_id() and radical.kanshitsu == 한실_무시) 	
                                     pro_dethrone_list.add(radical);

	                       }

                                 }


                       if (0 == pro_dethrone_list.count)
                       {

                                   @pro_dethrone1 = pk::get_person(무장_문관);
                                   @pro_dethrone2 = pk::get_person(무장_무관);	
                                   @pro_dethrone3 = pk::get_person(무장_선신);	
                                   @pro_dethrone4 = pk::get_person(무장_귀족);	
                                   @pro_dethrone5 = pk::get_person(무장_부호);	
                                   @pro_dethrone6 = pk::get_person(무장_학자);
								   
	                   }

                       else if (0 < pro_dethrone_list.count)
                       {
	                      pro_dethrone_list.sort(function(a, b)
	                      {				
	                      return float(( a.loyalty * 30 ) / (1 +  (4 * a.rank))) > float(( b.loyalty * 30 ) / (1 + (4 * b.rank)));
 
	                      });

                                   @pro_dethrone1 = 0 < pro_dethrone_list.count ? pro_dethrone_list[0] : pk::get_person(무장_문관);
                                   @pro_dethrone2 = 1 < pro_dethrone_list.count ? pro_dethrone_list[1] : pk::get_person(무장_무관);	
                                   @pro_dethrone3 = 2 < pro_dethrone_list.count ? pro_dethrone_list[2] : pk::get_person(무장_선신);	
                                   @pro_dethrone4 = 3 < pro_dethrone_list.count ? pro_dethrone_list[3] : pk::get_person(무장_귀족);	
                                   @pro_dethrone5 = 4 < pro_dethrone_list.count ? pro_dethrone_list[4] : pk::get_person(무장_부호);	
                                   @pro_dethrone6 = 5 < pro_dethrone_list.count ? pro_dethrone_list[5] : pk::get_person(무장_학자);

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


		   // 즉위 소식에 분개할 한실 중시 군주

		    pk::list<pk::force@> pro_han_force_list;
            pro_han_force_list.clear();			
			for (int i = 0; i < force_list.count; i++)
			{
	        pk::force@ pro_han_force = force_list[i];
	        pk::person@ pro_han_kunshu_group = pk::get_person(pk::get_kunshu_id(pro_han_force));
            if (pro_han_force.get_id() != force.get_id() and pro_han_kunshu_group.kanshitsu == 한실_중시) 	
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

		   // 이벤트 실행 
		   // 황제가 없을때		   
			if (emperor is null)
			{		   
		      pk::scene(pk::scene_t(scene_황제즉위_천자없음));
			  
			          if (force.title == 작위_황제 and 타국군주_반응보기)
			          {							  										   			   
						  pk::scene(pk::scene_t(scene_황제즉위_타국군주반응));					
			          }				  
			}
			
		   // 황제가 아직 있고 군주가 옹립중	   
            else if (emperor !is null and pk::is_protecting_the_emperor(force))
			{	
		   // 군주가 한실 중시이면서 황제의 최우선 친애무장인 경우 양위 		
               if ( (양위_조건 == 0 and kunshu.kanshitsu == 한실_중시 and pk::is_like(emperor, kunshu.get_id())) or (양위_조건 == 1 and kunshu.kanshitsu == 한실_중시) )
			   {
				      pk::scene(pk::scene_t(scene_황제즉위_천자가양위));
					  
			          if (force.title == 작위_황제 and 타국군주_반응보기)
			          {							  
						  pk::scene(pk::scene_t(scene_황제즉위_타국군주반응));					
			          }					  
			   }
		   // 그외 다른 경우들...군주가 한실 중시가 아니거나 황제의 친애무장이 아닌 경우 강제 폐위			   
               else
			   {
				      pk::scene(pk::scene_t(scene_황제즉위_천자를강제폐위));
					  
			          if (force.title == 작위_황제 and 타국군주_반응보기)
			          {							  
                         if (emperor.mibun == 신분_사망)
			              {	
			  	          pk::scene(pk::scene_t(scene_황제처형_타국군주반응));						  
			              }	
                         else
						  pk::scene(pk::scene_t(scene_황제즉위_타국군주반응));
					
			           }						  
					  
			          if (강제폐위로_플레이어_혐오)
					      pk::scene(pk::scene_t(scene_한실주의자의_혐오));


			   }			   

			}

            pk::scene(pk::scene_t(scene_화면_복귀));

			if (force.title == 작위_황제)		
			{
		  			   
		   // 히스토리 로그 기록
			  string emperor_title = pk::decode(pk::get_name(pk::get_title(작위_황제)));
		      pk::history_log(pk::get_current_turn_force_id(), force.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x(이)가 \x1b[1x{}\x1b[0x에 등극" , pk::decode(pk::get_name(kunshu)), emperor_title ) ) );
		      pk::history_log(pk::get_current_turn_force_id(), force.color, pk::encode(pk::format("\x1b[1x{}\x1b[0x(이)가 국호를 \x1b[2x{}\x1b[0x(으)로 제정" , pk::decode(pk::get_name(kunshu))  , pk::decode(pk::get_name(pk::get_kokugou(force.kokugou))) ) ) );
			}

		  }


		} // callback_emperor
		
	
		
		void scene_황제즉위_천자없음()
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

			pk::council(kunshu, subject1, subject2, subject3, subject4, subject5, subject6, pk::council_t(council_황제즉위_천자없음_설득));




		   // 국호 결정 순간


			pk::background(13);
			pk::fade(255);
			pk::message_box(pk::encode("주공, 국호는 \x1b[1x한\x1b[0x을 계승하시겠습니까?"), subject1);

				array<string> answers =
				{
					pk::encode("\x1b[1x한\x1b[0x을 계승한다."),
					pk::encode("새로운 국호를 정한다.")
				};
				
				int n = pk::choose(pk::encode("국호에 대한 방침을 결정하십시오."), answers);
		if (n == 0) 
                                   kokugou_han();

		if (n == 1) 
                  {
			  pk::message_box(pk::encode("그렇다면 새로운 국호를 정하셔야 할 듯 합니다, 폐하."), subject1);					  
		      kokugou_0();

				  if (한조중시가신_충성감소)
                     {
                                    // 세력 내 한왕실 옹호주의자들 충성도 변화
                                 pk::list<pk::person@> loyalist_list;
                                 loyalist_list.clear();									 
                                 auto member_list = pk::list_to_array(pk::get_person_list(force, pk::mibun_flags(신분_도독,신분_태수,신분_일반) ) );

                                 if (0 < member_list.length)
                                  {
           	                      for (int j = 0; j < member_list.length; j++)
           	                      {
	                       pk::person@ loyalist = member_list[j];	
                                     if (pk::is_alive(loyalist) and loyalist.get_id() != kunshu.get_id() and loyalist.kanshitsu == 한실_중시
									 and loyalist.get_id() != subject1.get_id() and loyalist.get_id() != subject2.get_id() and loyalist.get_id() != subject3.get_id()
									 and loyalist.get_id() != subject4.get_id() and loyalist.get_id() != subject5.get_id() and loyalist.get_id() != subject6.get_id()) 	
           	                        {
			// 군주를 친애하거나 부부, 의형제, 부모자식 관계일 경우 감소하지 않음
			if (func_relation_check(loyalist, kunshu) == 0 and func_mutual_relation_check(loyalist, kunshu) == 0)
           	                        {
                                       loyalist_list.add(loyalist);
		          pk::add_loyalty(loyalist, -국호변경_충성감소);

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

			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x의 새로운 국호를 도입한 결정에 충격을 받아, 한조를 중시하는 대신들의 충성도가 감소하였다.", kunshuname )));	
			pk::message_box(pk::encode("아....이게 무슨 일인가! 주공! \x1b[1x한\x1b[0x의 명맥을 다시 되살리셔야 합니다. 다시 한번 생각을!"), maverick1);
	                   }

                     } 

                } 

			pk::fade(0);
			pk::sleep();
			pk::background(18);	
			pk::fade(255);
            pk::play_bgm(19);			

            pk::play_se(6);
			
			string emperor_title = pk::decode(pk::get_name(pk::get_title(작위_황제))); 
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 마침내 \x1b[1x{}\x1b[0x의 자리에 올랐다.", kunshuname, emperor_title)));
			pk::play_se(10);
			pk::cutin(21);
			pk::play_se(35);			

		   // 작위 변경

		      force.title = 작위_황제;
		      force.update();

			pk::message_box(pk::encode("이 모든 것이 다 여러분들 덕분이오."), kunshu);
			pk::message_box(pk::encode("감축드리옵니다, 폐하!"), subject1);
			pk::message_box(pk::encode("온 백성들도 호응하고 있습니다, 폐하!"), subject4);
			pk::message_box(pk::encode("앞으로도 여러분들이 날 힘써 도와주시오."), kunshu);
			pk::message_box(pk::encode("폐하를 위해 어찌 노력을 다하지 않을 수 있겠습니까!"), subject5);
			pk::message_box(pk::encode("이제 천하통일이라는 과업만이 남았군요."), subject3);
			pk::message_box(pk::encode("폐하의 위엄을 사해에 널리 떨치겠습니다. 감축드리옵니다."), subject6);
			pk::message_box(pk::encode("다시 한번 감축드리옵니다."), subject2);


			pk::background(19);
	
                                           string current_year = pk::get_year();	
                                           string current_month = pk::get_month();
                                           string current_day = pk::get_day();			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x년 \x1b[1x{}\x1b[0x월 \x1b[1x{}\x1b[0x일.\n\x1b[1x{}\x1b[0x(은)는 마침내 \x1b[1x{}\x1b[0x의 자리에 올랐다.\n국호는 \x1b[1x{}\x1b[0x.", current_year, current_month, current_day, kunshuname, emperor_title , pk::decode(pk::get_name(pk::get_kokugou(force.kokugou))) )));
					
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		
		} // void scene_황제즉위_천자없음()
		
		

		void council_황제즉위_천자없음_설득()
		{

			pk::person@ subject1 = pk::is_alive(senior1) ? senior1 : pk::get_person(무장_문관);
			pk::person@ subject2 = pk::is_alive(senior2) ? senior2 : pk::get_person(무장_무관);
			pk::person@ subject3 = pk::is_alive(senior3) ? senior3 : pk::get_person(무장_선신);

			pk::person@ subject4 = pk::is_alive(senior4) ? senior4 : pk::get_person(무장_귀족);
			pk::person@ subject5 = pk::is_alive(senior5) ? senior5 : pk::get_person(무장_부호);
			pk::person@ subject6 = pk::is_alive(senior6) ? senior6 : pk::get_person(무장_학자);

			string kunshuname = pk::decode(pk::get_name(kunshu));
			string subject1name = pk::decode(pk::get_name(subject1));
			string emperor_title = pk::decode(pk::get_name(pk::get_title(작위_황제))); 
			
            pk::message_box(pk::encode(pk::format("주공, \x1b[1x{}\x1b[0x의 자리에 오르십시오.", emperor_title)), subject1);				
			

			pk::message_box(pk::encode("그렇사옵니다. 제위에 오르소서."), subject3);
			pk::message_box(pk::encode("아니, 그 무슨 말이오? 나는 그럴 수 없소..."), kunshu);
			pk::message_box(pk::encode("주공..."), subject1);
			pk::message_box(pk::encode("이는 단지 사사로운 이익 때문이 아니옵니다..."), subject2);
			pk::message_box(pk::encode("천하가 새로운 물결을 원하고 있고 주공을 위해 각고의 노력을 한 가신들 역시 바라는 일이옵니다."), subject1); 
			pk::message_box(pk::encode("이러한 기대에 부응하지 못한다면 오히려 큰 우를 범하고 말 것입니다..."), subject3); 
			pk::message_box(pk::encode("음...."), kunshu);
				
            pk::message_box(pk::encode(pk::format("정 그렇다면 내 무슨 할 말이 있겠소... 알겠소... 내 \x1b[1x{}\x1b[0x가 되겠소.", emperor_title)), kunshu);
			
			pk::message_box(pk::encode("정말이옵니까! 잘 생각하셨습니다, 주공!"), subject1);
			pk::message_box(pk::encode("이 기쁜 소식을 빨리 천하에 알려야겠군요!"), subject3);
			pk::message_box(pk::encode("모든 장수들에게 큰 힘이 될 것입니다!"), subject2);

		} // council_황제즉위_천자없음_설득()



		void scene_황제즉위_천자가양위()
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

	
			
			pk::move_screen(building.get_pos());
            pk::play_bgm(3);
			pk::message_box(pk::encode(pk::format("어느 날, \x1b[1x{}\x1b[0x에게 황제의 사자가 찾아왔다.", kunshuname)));
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x님, 황제께서 찾으십니다." , kunshuname)), pk::get_person(무장_사자));
			pk::message_box(pk::encode("갑자기 폐하께서...?"), kunshu);


			pk::diplomacy(emperor, null, kunshu, pk::diplomacy_t(diplomacy_천자가직접양위설득));



		   // 국호 결정 순간


			pk::background(13);
			pk::fade(255);
			pk::message_box(pk::encode("주공, 국호는 \x1b[1x한\x1b[0x을 계승하시겠습니까?"), subject1);

				array<string> answers =
				{
					pk::encode("\x1b[1x한\x1b[0x을 계승한다."),
					pk::encode("새로운 국호를 정한다.")
				};
				
				int n = pk::choose(pk::encode("국호에 대한 방침을 결정하십시오."), answers);
		if (n == 0) 
                                   kokugou_han();

		if (n == 1) 
                  {	
			  pk::message_box(pk::encode("그렇다면 새로운 국호를 정하셔야 할 듯 합니다, 폐하."), subject1);			  
		      kokugou_0();

				  if (한조중시가신_충성감소)
                     {
                                    // 세력 내 한왕실 옹호주의자들 충성도 변화
                                 pk::list<pk::person@> loyalist_list;
                                 loyalist_list.clear();									 
                                 auto member_list = pk::list_to_array(pk::get_person_list(force, pk::mibun_flags(신분_도독,신분_태수,신분_일반) ) );

                                 if (0 < member_list.length)
                                  {
           	                      for (int j = 0; j < member_list.length; j++)
           	                      {
	                       pk::person@ loyalist = member_list[j];	
                                     if (pk::is_alive(loyalist) and loyalist.get_id() != kunshu.get_id() and loyalist.kanshitsu == 한실_중시
									 and loyalist.get_id() != subject1.get_id() and loyalist.get_id() != subject2.get_id() and loyalist.get_id() != subject3.get_id()
									 and loyalist.get_id() != subject4.get_id() and loyalist.get_id() != subject5.get_id() and loyalist.get_id() != subject6.get_id()) 	
           	                        {
			// 군주를 친애하거나 부부, 의형제, 부모자식 관계일 경우 감소하지 않음
			if (func_relation_check(loyalist, kunshu) == 0 and func_mutual_relation_check(loyalist, kunshu) == 0)
           	                        {
                                       loyalist_list.add(loyalist);
		          pk::add_loyalty(loyalist, -국호변경_충성감소_양위);

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

			pk::message_box(pk::encode(pk::format("비록 \x1b[1x{}\x1b[0x의 새로운 국호를 도입한 결정은 한조를 중시하는 대신들에게 호응을 얻진 못했으나...", kunshuname )));	
			pk::message_box(pk::encode("....안타깝지만 폐하께서 직접 \x1b[1x한\x1b[0x의 운명을 결정지으신거니, 받아들이는 수 밖에..."), maverick1);
			pk::message_box(pk::encode(pk::format("평화적인 양위였기 때문에, \x1b[1x{}\x1b[0x의 새로운 국호에 크게 반발하는 대신들은 없었다고 한다...", kunshuname )));		

                       }
                     } 

                } 

			pk::fade(0);
			pk::sleep();
			pk::background(18);
			pk::fade(255);			
            pk::play_bgm(19);

            pk::play_se(6);

			string emperor_title = pk::decode(pk::get_name(pk::get_title(작위_황제))); 
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 마침내 \x1b[1x{}\x1b[0x의 자리에 올랐다.", kunshuname, emperor_title)));
			pk::play_se(10);
			pk::cutin(21);
			pk::play_se(35); 			


			// 후한 황제 퇴위
		
            pk::get_scenario().emperor = 무장_끝;
            emperor.update();


		   // 작위 변경

		      force.title = 작위_황제;
		      force.update();

			pk::message_box(pk::encode("이 모든 것이 다 여러분들 덕분이오."), kunshu);
			pk::message_box(pk::encode("감축드리옵니다, 폐하!"), subject1);
			pk::message_box(pk::encode("온 백성들도 호응하고 있습니다, 폐하!"), subject4);
			pk::message_box(pk::encode("앞으로도 여러분들이 날 힘써 도와주시오."), kunshu);
			pk::message_box(pk::encode("폐하를 위해 어찌 노력을 다하지 않을 수 있겠습니까!"), subject5);
			pk::message_box(pk::encode("이제 천하통일이라는 과업만이 남았군요."), subject3);
			pk::message_box(pk::encode("폐하의 위엄을 사해에 널리 떨치겠습니다. 감축드리옵니다."), subject6);
			pk::message_box(pk::encode("다시 한번 감축드리옵니다."), subject2);


			pk::background(19);

                                           string current_year = pk::get_year();	
                                           string current_month = pk::get_month();
                                           string current_day = pk::get_day();			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x년 \x1b[1x{}\x1b[0x월 \x1b[1x{}\x1b[0x일.\n\x1b[1x{}\x1b[0x(은)는 마침내 \x1b[1x{}\x1b[0x의 자리에 올랐다.\n국호는 \x1b[1x{}\x1b[0x.", current_year, current_month, current_day, kunshuname, kunshuname , pk::decode(pk::get_name(pk::get_kokugou(force.kokugou))) )));
					
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);
		
		} // void scene_황제즉위_천자가양위()


		void diplomacy_천자가직접양위설득()
		{
			pk::person@ subject1 = pk::is_alive(senior1) ? senior1 : pk::get_person(무장_문관);
			pk::person@ subject2 = pk::is_alive(senior2) ? senior2 : pk::get_person(무장_무관);
			pk::person@ subject3 = pk::is_alive(senior3) ? senior3 : pk::get_person(무장_선신);

			pk::person@ subject4 = pk::is_alive(senior4) ? senior4 : pk::get_person(무장_귀족);
			pk::person@ subject5 = pk::is_alive(senior5) ? senior5 : pk::get_person(무장_부호);
			pk::person@ subject6 = pk::is_alive(senior6) ? senior6 : pk::get_person(무장_학자);

			string kunshuname = pk::decode(pk::get_name(kunshu));
			string subject1name = pk::decode(pk::get_name(subject1));			

			pk::message_box(pk::encode("폐하, 절 찾으셨다 들었습니다."), kunshu);
            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x공! 잘 와주었소. 내 그대에게 전할 말이 있소.", kunshuname)), emperor);

            pk::message_box(pk::encode(pk::format("짐은 \x1b[1x{}\x1b[0x공, 그대에게 양위를 하려 하오.", kunshuname)), emperor);
			pk::message_box(pk::encode("아니...폐하, 갑자기 무슨 말씀이신지..."), kunshu);
			
            pk::message_box(pk::encode(pk::format("짐은 오랫동안 그대, \x1b[1x{}\x1b[0x공의 모습을 보며, 그대가 나 대신 이 나라를 안정시킬 적임자임을 확신했소. 부디 내 진심을 알아주길 바라오.", kunshuname)), emperor);			
			
			pk::message_box(pk::encode("주공, 천자깨서 심사숙고 끝에 내리신 결정이시니 제위에 오르십시오."), subject1);
			pk::message_box(pk::encode("맞습니다. 부디 천하를 안정시키기 위한 결단을!"), subject2);
			pk::message_box(pk::encode("음...."), kunshu);
									
			pk::message_box(pk::encode("어쩔 수 없군요...폐하의 뜻에 따르겠습니다."), kunshu);
			pk::message_box(pk::encode("잘 생각하셨소! 이제 천하가 평안하겠구려."), emperor);
			pk::message_box(pk::encode("이 기쁜 소식을 빨리 천하에 알려야겠군요!"), subject1);
			pk::message_box(pk::encode("모든 장수들에게 큰 힘이 될 것입니다!"), subject2);

		} // diplomacy_천자가직접양위설득()


		void scene_황제즉위_천자를강제폐위()
		{
				@force = pk::get_force(kunshu.get_force_id());
			pk::person@ subject1 = pk::is_alive(pro_dethrone1) ? pro_dethrone1 : pk::get_person(무장_문관);
			pk::person@ subject2 = pk::is_alive(pro_dethrone2) ? pro_dethrone2 : pk::get_person(무장_무관);
			pk::person@ subject3 = pk::is_alive(pro_dethrone3) ? pro_dethrone3 : pk::get_person(무장_선신);

			pk::person@ subject4 = pk::is_alive(pro_dethrone4) ? pro_dethrone4 : pk::get_person(무장_귀족);
			pk::person@ subject5 = pk::is_alive(pro_dethrone5) ? pro_dethrone5 : pk::get_person(무장_부호);
			pk::person@ subject6 = pk::is_alive(pro_dethrone6) ? pro_dethrone6 : pk::get_person(무장_학자);

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

			pk::fade(0);
			pk::sleep();
			pk::background(53);
			pk::fade(255);
			
            pk::play_bgm(15);
			
			string emperor_title = pk::decode(pk::get_name(pk::get_title(작위_황제))); 
									
			pk::message_box(pk::encode("주공, 지금의 천자를 폐하시고 직접 황위에 오르십시오."), subject1);
			pk::message_box(pk::encode("아니, 그게 무슨 소리요?"), kunshu);			
            pk::message_box(pk::encode(pk::format("주공의 위명이 사해를 떨치고 있지만, 천하 통일의 명분을 내세우려면 결국 \x1b[1x{}\x1b[0x로 즉위하셔야 합니다.", emperor_title)), subject2);						
			pk::message_box(pk::encode("그렇다 해도...너무나 갑작스런 이야기가 아니오?"), kunshu);		
            pk::message_box(pk::encode(pk::format("주공께선 천하의 6할 가까이를 차지하셨지만 \x1b[1x{}\x1b[0x의 자리에 오르시지 않는다면, 결국은 한의 천하이지, 주공의 천하가 아닌 것입니다.", emperor_title)), subject3);			
			pk::message_box(pk::encode("......."), kunshu);	
			pk::message_box(pk::encode("더 시간을 지체하셨다간 지금의 천자의 자리만 공고해집니다. 주공, 부디 결단을..."), subject4);
			pk::message_box(pk::encode("한의 시대는 저물었고, 이제 새로운 시대로 나아가야 합니다."), subject5);			
			pk::message_box(pk::encode("맞습니다. 천하에 새로운 위명을 떨쳐보여야 합니다. 주공, 어서 결단을!"), subject6);				
			
			
			
				array<string> response =
				{
					pk::encode("아무런 반응을 보이지 않는다."),
					pk::encode("천자를 폐위하고, 스스로 직접 제위에 오른다.")
				};
				
				int m = pk::choose(pk::encode("천자를 폐위하고 스스로 황제가 될지 여부를 결정하십시오."), response);
				
		if (m == 0) 
		   {			
                                   천자강제폐위_침묵();
								   
			if (황제협박_매력감소) 
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

                     if (target_member.base_stat[무장능력_매력] > 협박_매력_감소량)
		     	     {
				     target_member.base_stat[무장능력_매력] = target_member.base_stat[무장능력_매력] - 협박_매력_감소량;	
                     target_member.update();
		             }
                     else if (target_member.base_stat[무장능력_매력] <= 협박_매력_감소량)
		     	     {
				     target_member.base_stat[무장능력_매력] = 1;	
                     target_member.update();
		             }	
					 
				  person_name_list.insertLast(pk::format("\x1b[1x{}\x1b[0x", pk::decode(pk::get_name(target_member))));
		 					   
		           }
				   
				  string person_names = join(person_name_list, ", "); 
		        		
			     pk::play_se(11);
			     pk::message_box(pk::encode(pk::format("황제를 협박한 대가로 \x1b[2x{}\x1b[0x - 이 \x1b[2x{}\x1b[0x명의 평판이 실추, 매력이 감소하였습니다.", person_names, target_list.count)));		
		        }
					 
		      }								   
		   }

		if (m == 1) 
		   {			
                                   천자강제폐위_직접처리();			

				if (황제협박_매력감소) 
		     	{	
			     pk::play_se(11);
			     pk::message_box(pk::encode(pk::format("황제를 협박한 대가로 \x1b[2x{}\x1b[0x의 평판이 실추, 매력이 감소하였습니다.", kunshuname)));		

                     if (kunshu.base_stat[무장능력_매력] > 협박_매력_감소량)
		     	     {
				     kunshu.base_stat[무장능력_매력] = kunshu.base_stat[무장능력_매력] - 협박_매력_감소량;	
                     kunshu.update();
		             }
                     else if (kunshu.base_stat[무장능력_매력] <= 협박_매력_감소량)
		     	     {
				     kunshu.base_stat[무장능력_매력] = 1;	
                     kunshu.update();
		             }
						 
		        }			
		   }




		   // 국호 결정 순간

			pk::background(41);	
			pk::fade(255);
			pk::message_box(pk::encode("폐하, \x1b[1x한\x1b[0x은 이제 역사 속으로 사라진 나라이옵니다."), subject1);

			pk::message_box(pk::encode("그렇다면 새로운 국호를 정하셔야 할 듯 합니다, 폐하."), subject2);
		      kokugou_0();
			  
			  
			pk::fade(0);
			pk::sleep();
			pk::background(18);
			pk::fade(255);

            pk::play_bgm(22);							 

            pk::play_se(6);
			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 마침내 \x1b[1x{}\x1b[0x의 자리에 올랐다.", kunshuname, emperor_title)));
			pk::play_se(10);

			pk::cutin(21);
			pk::play_se(35); 

			// 후한 황제 퇴위 + 황제가 플레이어 군주를 혐오하기 시작
			
		    if (!pk::is_dislike(emperor, kunshu.get_id()) and pk::get_num_dislikes(emperor) <= 4)
            pk::add_dislike(emperor, kunshu.get_id());				
		
            pk::get_scenario().emperor = 무장_끝;
            emperor.update();


		   // 작위 변경

		      force.title = 작위_황제;
		      force.update();

			pk::message_box(pk::encode("이 모든 것이 다 여러분들 덕분이오."), kunshu);
			pk::message_box(pk::encode("감축드리옵니다, 폐하!"), subject1);
			pk::message_box(pk::encode("온 백성들도 호응하고 있습니다, 폐하!"), subject4);
			pk::message_box(pk::encode("앞으로도 여러분들이 날 힘써 도와주시오."), kunshu);
			pk::message_box(pk::encode("폐하를 위해 어찌 노력을 다하지 않을 수 있겠습니까!"), subject5);
			pk::message_box(pk::encode("이제 천하통일이라는 과업만이 남았군요."), subject3);
			pk::message_box(pk::encode("폐하의 위엄을 사해에 널리 떨치겠습니다. 감축드리옵니다."), subject6);
			pk::message_box(pk::encode("다시 한번 감축드리옵니다."), subject2);


			pk::background(19);

                                           string current_year = pk::get_year();	
                                           string current_month = pk::get_month();
                                           string current_day = pk::get_day();			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x년 \x1b[1x{}\x1b[0x월 \x1b[1x{}\x1b[0x일.\n\x1b[1x{}\x1b[0x(은)는 마침내 \x1b[1x{}\x1b[0x의 자리에 올랐다.\n국호는 \x1b[1x{}\x1b[0x.", current_year, current_month, current_day, kunshuname, emperor_title , pk::decode(pk::get_name(pk::get_kokugou(force.kokugou))) )));

			pk::message_box(pk::encode("수백년의 한조가 나의 세대에서 망하다니...선조들을 무슨 낯으로 뵐 수 있으랴...부끄럽도다..."), emperor);

					
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);



		if (황제_처형) 
		   {			

			string emperorname = pk::decode(pk::get_name(emperor));			

   
			pk::move_screen(building.get_pos());
            pk::play_bgm(14);
			pk::message_box(pk::encode(pk::format("하지만, 이것으로 끝난 것이 아니었다...\x1b[1x{}\x1b[0x에게 \x1b[2x{}\x1b[0x(을)를 비롯한 가신들은 더 충격적인 권유를 하게 되는데...", kunshuname, subject1name)));
			pk::message_box(pk::encode("폐하, 폐위된 황제를 어떻게 하시겠습니까...?"), subject1);

			pk::council(kunshu, subject1, subject2, subject3, subject4, subject5, subject6, pk::council_t(council_천자처형_논의));
	
	

				array<string> execution =
				{
					pk::encode("일단 폐위된 황제를 살려둔다."),
					pk::encode("폐위된 황제를 처형하여 후환을 막는다.")
				};
				
				int k = pk::choose(pk::encode("천자에 대한 처우를 결정하십시오."), execution);

		if (k == 0) 
		   {			
			pk::message_box(pk::encode("지금 그렇게까지 극단적으로 행동할 필요가 있겠소? 그리고 새로이 나라의 주인이 바뀌었으니 국가를 안정시키는 게 급선무요. 더는 말하지 마오."), kunshu);
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x는 옛 황제를 일단 살려두기로 하며, \x1b[2x{}\x1b[0x(을)를 비롯한 가신들의 권유를 물리쳤다.", kunshuname, subject1name)));			

		   }			

		if (k == 1) 
		   {			
			pk::message_box(pk::encode("...맞는 말이오. 후환의 싹은 제거해둬야겠지..."), kunshu);
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x는 \x1b[2x{}\x1b[0x(을)를 비롯한 가신들의 권유대로 옛 황제를 처형하기로 결정했다.", kunshuname, subject1name)));			

			pk::fade(0);
			pk::sleep();
			pk::background(55);
			pk::fade(255);

            pk::message_box(pk::encode(pk::format("이럴 수가...어떻게 자신의 옛 군주를 죽이려 한단 말인가, \x1b[1x{}\x1b[0x!...", kunshuname )), emperor);
			pk::message_box(pk::encode("안됐지만 어쩔 수 없소이다."), subject1);
            pk::message_box(pk::encode(pk::format("아아...나라를 빼앗은 마당에 내 목숨까지 가져가겠다는 건가...\x1b[1x{}\x1b[0x, 이 저주 받을 놈!", kunshuname )), emperor);
			pk::message_box(pk::encode("도부수는 어서 베어라."), subject1);	
			pk::message_box(pk::encode("에잇!"), pk::get_person(무장_무관));	
			pk::cutin(15);	
            pk::message_box(pk::encode(pk::format("내가 죽더라도 한조 수백년 역사가 네 육신을 짓누를 것이다! \x1b[1x{}\x1b[0x!", kunshuname )), emperor);			

                                           string current_year = pk::get_year();	
                                           string current_month = pk::get_month();
                                           string current_day = pk::get_day();			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x년 \x1b[1x{}\x1b[0x월 \x1b[1x{}\x1b[0x일.\n\x1b[1x{}\x1b[0x, \x1b[1x{}\x1b[0x에게 처형당하다.", current_year, current_month, current_day, emperorname, kunshuname )));
			pk::play_se(5);	
            pk::kill(emperor, kunshu, pk::get_hex_object(emperor.location), null, 2);			
					
				if (황제처형_매력감소)
		     	{	
			     pk::play_se(11);
			     pk::message_box(pk::encode(pk::format("황제를 처형한 대가로 \x1b[2x{}\x1b[0x의 평판이 실추, 매력이 감소하였습니다.", kunshuname)));		

                     if (kunshu.base_stat[무장능력_매력] > 처형_매력_감소량)
		     	     {
				     kunshu.base_stat[무장능력_매력] = kunshu.base_stat[무장능력_매력] - 처형_매력_감소량;	
                     kunshu.update();
		             }
                     else if (kunshu.base_stat[무장능력_매력] <= 처형_매력_감소량)
		     	     {
				     kunshu.base_stat[무장능력_매력] = 1;	
                     kunshu.update();
		             }
						 
		        }					 
		   }			
 
			
  
		   }




				  if (한조중시가신_충성감소)
                     {						 
                                    // 세력 내 한왕실 옹호주의자들 충성도 변화
                                 pk::list<pk::person@> loyalist_list;
                                 loyalist_list.clear();									 
                                 auto member_list = pk::list_to_array(pk::get_person_list(force, pk::mibun_flags(신분_도독,신분_태수,신분_일반) ) );

                                 if (0 < member_list.length)
                                  {
           	                      for (int j = 0; j < member_list.length; j++)
           	                      {
	                       pk::person@ loyalist = member_list[j];	
                                     if (pk::is_alive(loyalist) and loyalist.get_id() != kunshu.get_id() and loyalist.kanshitsu == 한실_중시) 	
           	                        {
			// 군주를 친애하거나 부부, 의형제, 부모자식 관계일 경우 감소하지 않음
			if (func_relation_check(loyalist, kunshu) == 0 and func_mutual_relation_check(loyalist, kunshu) == 0
				and loyalist.get_id() != subject1.get_id() and loyalist.get_id() != subject2.get_id() and loyalist.get_id() != subject3.get_id()
				and loyalist.get_id() != subject4.get_id() and loyalist.get_id() != subject5.get_id() and loyalist.get_id() != subject6.get_id())
           	                        {
                                       loyalist_list.add(loyalist);
                                    if (emperor.mibun != 신분_사망)									   
		                                pk::add_loyalty(loyalist, -폐위_충성감소);
									
                                    else if (emperor.mibun == 신분_사망)									
		                                pk::add_loyalty(loyalist, -폐위와처형_충성감소);

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
                                    if (emperor.mibun != 신분_사망)
           	                        {										
									pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x가 한 제국을 멸망시킨 소식에 한조를 중시하는 대신들의 분노가 쏟아졌다.", kunshuname )));	
									pk::message_box(pk::encode("어떻게 이런 일이 있을 수 있단 말인가! 내가 모시는 주인이 한을 멸망시키다니!...."), maverick1);
                                    }

                                    else if (emperor.mibun == 신분_사망)
          	                        {										
									pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x가 한 제국을 멸망시킨 것도 모자라 황제를 처형했다는 소식에 한조를 중시하는 대신들의 분노와 경악이 치솟았다.", kunshuname )));	
									pk::message_box(pk::encode("이..이게 말이 되는가? 내가 모시는 주인이 한을 멸망시킨 것도 모자라 황제를 처형하다니!...."), maverick1);
                                    }
                       }									
                     } 	

		


		} // void scene_황제즉위_천자를강제폐위()



		void council_천자처형_논의()
		{
				@force = pk::get_force(kunshu.get_force_id());			
			pk::person@ subject1 = pk::is_alive(pro_dethrone1) ? pro_dethrone1 : pk::get_person(무장_문관);
			pk::person@ subject2 = pk::is_alive(pro_dethrone2) ? pro_dethrone2 : pk::get_person(무장_무관);
			pk::person@ subject3 = pk::is_alive(pro_dethrone3) ? pro_dethrone3 : pk::get_person(무장_선신);

			pk::person@ subject4 = pk::is_alive(pro_dethrone4) ? pro_dethrone4 : pk::get_person(무장_귀족);
			pk::person@ subject5 = pk::is_alive(pro_dethrone5) ? pro_dethrone5 : pk::get_person(무장_부호);
			pk::person@ subject6 = pk::is_alive(pro_dethrone6) ? pro_dethrone6 : pk::get_person(무장_학자);

			string kunshuname = pk::decode(pk::get_name(kunshu));
			string subject1name = pk::decode(pk::get_name(subject1));
			
			pk::message_box(pk::encode("폐하, 옛 황제를 그대로 두면 후환이 생길까 두렵습니다."), subject1);
			pk::message_box(pk::encode("그렇사옵니다. 분명 옛 한실에 대한 미련이 남은 자들이 옹립하려 들 수도 있사옵니다."), subject2);		
			pk::message_box(pk::encode("그렇지만 벌서부터 너무 걱정하는 것이 아니오? 아직 폐위된 옛 황제가 무슨 움직임을 보인 것도 아니잖소."), kunshu);			
			pk::message_box(pk::encode("허나 옛 황제가 살아 존재한다는 사실 만으로도 한실추종자들에겐 다른 마음을 품게 될 여지를 주게 되는 겁니다."), subject3);
			pk::message_box(pk::encode("그리고 폐하께서도 아시다시피 그들에게 가장 중요한 것은 명분아니겠습니까."), subject4);			
			pk::message_box(pk::encode("....음..."), kunshu);
			pk::message_box(pk::encode("결국 그들은 폐하께서 천하를 통일하려는 중요한 시기에 옛 황제를 내세워 대사에 지장을 줄 것입니다."), subject5);
			pk::message_box(pk::encode("폐하, 인간적인 정으로 가볍게 넘기실 일이 아닙니다. 지금 내친 김에 반란의 싹을 확실히 제거해야 합니다."), subject6);				

		} // void council_천자처형_논의()


		void 천자강제폐위_침묵()
		{	
				@force = pk::get_force(kunshu.get_force_id());
			pk::person@ subject1 = pk::is_alive(pro_dethrone1) ? pro_dethrone1 : pk::get_person(무장_문관);	
			pk::person@ subject2 = pk::is_alive(pro_dethrone2) ? pro_dethrone2 : pk::get_person(무장_무관);
			pk::person@ subject3 = pk::is_alive(pro_dethrone3) ? pro_dethrone3 : pk::get_person(무장_선신);
			
			string kunshuname = pk::decode(pk::get_name(kunshu));
			string subject1name = pk::decode(pk::get_name(subject1));
			
				pk::message_box(pk::encode("......."), kunshu);
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x는 아무런 말도 하지 않는다...", kunshuname)));					

			pk::message_box(pk::encode("주공, 걱정하지 마십시오. 저희가 알아서 하겠습니다."), subject1);
			pk::message_box(pk::encode("이 일은 저희에게 일임해주시기 바랍니다."), subject2);
			pk::message_box(pk::encode("주공께서는 가만히 지켜보시기만 하면 됩니다."), subject3);			

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

			
			pk::message_box(pk::encode(pk::format("얼마 후, \x1b[2x{}\x1b[0x - 이들 \x1b[1x{}\x1b[0x의 가신들이 황제를 알현했다.", person_names, kunshuname)));

			pk::council(emperor, pk::get_person(무장_사자), subject1, subject2, subject3, null, null, pk::council_t(council_천자강제폐위_가신들천자협박));

		} // 천자강제폐위_침묵()


		void council_천자강제폐위_가신들천자협박()
		{	
			pk::person@ subject1 = pk::is_alive(pro_dethrone1) ? pro_dethrone1 : pk::get_person(무장_문관);	
			pk::person@ subject2 = pk::is_alive(pro_dethrone2) ? pro_dethrone2 : pk::get_person(무장_무관);
			pk::person@ subject3 = pk::is_alive(pro_dethrone3) ? pro_dethrone3 : pk::get_person(무장_선신);
			
			string kunshuname = pk::decode(pk::get_name(kunshu));			
					  																																																
			pk::message_box(pk::encode("짐을 만나고 싶다는 얘기는 들었소. 그래, 그대들은 무슨 일로 왔소?"), emperor);	
			pk::message_box(pk::encode(pk::format("폐하, 이제 \x1b[1x{}\x1b[0x님께 자리를 물려주심이 어떻겠습니까?" , kunshuname)), subject1);			
			pk::message_box(pk::encode("!!!!"), emperor);
			pk::message_box(pk::encode(pk::format("한의 명운은 이미 다했고, 천하를 새로이 이어나갈 덕을 가진 \x1b[1x{}\x1b[0x님이 이제 황위에 오르셔야 한다고 봅니다." , kunshuname)), subject2);
			pk::message_box(pk::encode("감히! 뉘 앞이라고 이런 무엄한 말들을 하는 게요!"), pk::get_person(무장_사자));				
			pk::message_box(pk::encode("그대들은 어찌 나에게 이럴 수 있는가!"), emperor);
			pk::message_box(pk::encode("어차피 대세는 정해졌고 시대의 물결은 거스를 수 없는 것입니다. 폐하께서도 이제 그만 인정하시지요."), subject3);	
			pk::message_box(pk::encode("..............너희들은 하늘이 두렵지도 않느냐..."), emperor);	
			pk::message_box(pk::encode("이 황궁 안에 돌이킬 수 없는 사태가 벌어지지 않는 걸 원하신다면, 조용히 받아들이시는 게 좋을 겁니다."), subject1);		
			pk::message_box(pk::encode(".......알겠소. 경들 뜻대로 하시오."), emperor);			
			pk::message_box(pk::encode("결정 잘 하셨습니다. 동의하신 걸로 알고, 절차를 시작하겠습니다."), subject2);	
			pk::message_box(pk::encode("폐하...."), pk::get_person(무장_사자));				
			pk::message_box(pk::encode(".......무력한 내가 원망스럽구나 ...."), emperor);				

		} // void council_천자강제폐위_가신들천자협박()


		void 천자강제폐위_직접처리()
		{	
				@force = pk::get_force(kunshu.get_force_id());
			pk::person@ subject1 = pk::is_alive(pro_dethrone1) ? pro_dethrone1 : pk::get_person(무장_문관);	
			pk::person@ subject2 = pk::is_alive(pro_dethrone2) ? pro_dethrone2 : pk::get_person(무장_무관);
			pk::person@ subject3 = pk::is_alive(pro_dethrone3) ? pro_dethrone3 : pk::get_person(무장_선신);
			
			string kunshuname = pk::decode(pk::get_name(kunshu));
			string subject1name = pk::decode(pk::get_name(subject1));
            string emperor_title = pk::decode(pk::get_name(pk::get_title(작위_황제)));			
			
				pk::message_box(pk::encode("알겠소이다. 내 뜻은 정해졌소. 내가 직접 황제께 수백년 한조의 자격을 묻겠소."), kunshu);
			    pk::message_box(pk::encode(pk::format("드디어 \x1b[1x{}\x1b[0x는 직접 \x1b[1x{}\x1b[0x의 자리에 오를 뜻을 밝혔다!", kunshuname, emperor_title)));					

			pk::message_box(pk::encode("오오오...잘 생각하셨습니다, 주공!"), subject1);
			pk::message_box(pk::encode("역시! 이제야 천하에 제대로 된 나라가 들어서게 되겠군요!"), subject2);
			pk::message_box(pk::encode("지금의 황제께서도 이해하실겁니다."), subject3);			
						
			pk::fade(0);
			pk::sleep();
			pk::background(34);	
			pk::fade(255);	
			
			pk::message_box(pk::encode(pk::format("얼마 후, \x1b[1x{}\x1b[0x는 직접 황제를 알현했다.", kunshuname)));

			pk::diplomacy(emperor, null, kunshu, pk::diplomacy_t(diplomacy_천자를직접협박));

		} // 천자강제폐위_직접처리()	


		void diplomacy_천자를직접협박()
		{
		
			string kunshuname = pk::decode(pk::get_name(kunshu));	
			
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x공, 어서오시오. 그래, 오늘은 무슨 일로?" , kunshuname)), emperor);				
		    pk::message_box(pk::encode("외람된 말씀이지만, 폐하께서는 현재 천하의 물결을 어떻게 보고 계십니까?"), kunshu);
			pk::message_box(pk::encode("천하의 물결이라니?"), emperor);
			pk::message_box(pk::encode(pk::format("지금 한 제국의 기력은 그 끝을 향하고 있고, 이 \x1b[1x{}\x1b[0x가 전란에 휩쓸린 대륙을 안정케 한지 오래입니다." , kunshuname)), kunshu);				
			pk::message_box(pk::encode("....?"), emperor);
			
			pk::message_box(pk::encode(pk::format("천하의 물결을 살피건대, 순리대로 생각한다면 마땅히 이 \x1b[1x{}\x1b[0x가 천하를 다스리는 것이 맞지 않겠습니까?" , kunshuname)), kunshu);	
			pk::message_box(pk::encode("!!!!"), emperor);			
		    pk::message_box(pk::encode("폐하께서 진정 천하를 걱정하신다면, 천하를 옳은 주인에게로 돌려주시리라 생각합니다."), kunshu);				
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x공, 이 무슨 당치도 않은 소리요! 지금 나를...나를 끌어내리겠단 말이오?! 절대 그렇게 할 순 없소!" , kunshuname)), emperor);		
		    pk::message_box(pk::encode("당장의 감정에 치우치지 마시고, 훗날의 일들을 잘 생각하시길 바랍니다."), kunshu);		
			pk::message_box(pk::encode(pk::format("아아...\x1b[1x{}\x1b[0x, 당신은 기어코 이 한조를 무너뜨리려 하려는가..." , kunshuname)), emperor);	
		    pk::message_box(pk::encode("천하의 주인은 바뀌기 마련 아니오? 이제 운명은 정해졌소이다."), kunshu);			
			
			pk::message_box(pk::encode("........알겠소. 그대 뜻대로 하시오..."), emperor);			
		    pk::message_box(pk::encode("잘 생각하셨소이다. 그럼 이만."), kunshu);
			pk::message_box(pk::encode("......원통하도다...."), emperor);			

		} // void diplomacy_천자를직접협박()



        //----------------------------------------------------------------------------------
        //  국호 선택 창
        //----------------------------------------------------------------------------------



// 국호 선택

		void kokugou_han()
		{	
				@force = pk::get_force(kunshu.get_force_id());
			pk::person@ subject1 = pk::is_alive(senior1) ? senior1 : pk::get_person(무장_문관);	
			string kunshuname = pk::decode(pk::get_name(kunshu));

			pk::background(13);	
				pk::message_box(pk::encode("당연히 우리 모두 \x1b[1x한\x1b[0x의 자식들 아니겠소. 이 나라의 국호를 \x1b[1x한\x1b[0x으로 유지하고 한왕조를 계승하겠소!"), kunshu);
				force.kokugou = 국호_한; 
		                             force.update();

			pk::message_box(pk::encode("참으로 지당하신 말씀이옵니다, 폐하."), subject1);
			pk::background(12);	
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x{}\x1b[0x으로 유지하며 한왕실을 계승하였다.", kunshuname , pk::decode(pk::get_name(pk::get_kokugou(force.kokugou))) )));


                  if (한조중시가신_충성증가)
                  {					  

                                    // 세력 내 한왕실 옹호주의자들 충성도 변화
                                 pk::list<pk::person@> loyalist_list;
                                 loyalist_list.clear();								 
                                 auto member_list = pk::list_to_array(pk::get_person_list(force, pk::mibun_flags(신분_도독,신분_태수,신분_일반) ) );

                                 if (0 < member_list.length)
                                  {
           	                      for (int j = 0; j < member_list.length; j++)
           	                      {
	                       pk::person@ loyalist = member_list[j];	
                                     if (pk::is_alive(loyalist) and loyalist.get_id() != kunshu.get_id() and loyalist.kanshitsu == 한실_중시) 	
           	                        {
                                       loyalist_list.add(loyalist);
		          pk::add_loyalty(loyalist, 국호유지_충성증가);
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

			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x의 국호 유지 소식에 한조를 중시하는 대신들의 충성도가 증가하였다.", kunshuname )));
			pk::message_box(pk::encode("잘하셨습니다! 주공! \x1b[1x한\x1b[0x의 명맥이 이렇게 다시 살아나는 구나! 이렇게 통쾌할수가!"), maverick1);
                       }
                    }
	
		} // kokugou_han()


			
		void kokugou_0()
		{	
			pk::background(41);		
				@force = pk::get_force(kunshu.get_force_id());
			pk::person@ subject1 = pk::is_alive(senior1) ? senior1 : pk::get_person(무장_문관);	
			string kunshuname = pk::decode(pk::get_name(kunshu));

			array<string> kokugou_list =
			{
				pk::encode("위, 오, 촉, 진, 성"),
				pk::encode("하, 상, 주, 신, 연"),
				pk::encode("조, 제, 기, 노, 설"),
				pk::encode("서, 송, 소, 정, 허"),
				pk::encode("우, 양, 채, 당, 등"),
				pk::encode("수, 초, 월, 진, 조"),
			};
			
			int n = pk::choose(pk::encode("국호를 선택해주십시오."), kokugou_list);
			
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
				pk::encode("위"),
				pk::encode("오"),
				pk::encode("촉"),
				pk::encode("진"),
				pk::encode("성"),
				pk::encode("BACK"),
			};
			
			int n = pk::choose(pk::encode("국호를 선택해주십시오."), kokugou_list);
			
			if (n == 0)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x위\x1b[0x로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x위\x1b[0x로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x위\x1b[0x로 선포하노라!"), kunshu);
				force.kokugou = 국호_위; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x위\x1b[0x로 선포하였다.", kunshuname  )));				
			    }
		        if (k == 1) 			
				kokugou_1();
			}
			else if (n == 1)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x오\x1b[0x로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x오\x1b[0x로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x오\x1b[0x로 선포하노라!"), kunshu);
				force.kokugou = 국호_오; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x오\x1b[0x로 선포하였다.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_1();
			}
			else if (n == 2)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x촉\x1b[0x으로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x촉\x1b[0x으로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x촉\x1b[0x으로 선포하노라!"), kunshu);
				force.kokugou = 국호_촉; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x촉\x1b[0x으로 선포하였다.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_1();
			}
			else if (n == 3)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x진\x1b[0x으로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x진\x1b[0x으로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x진\x1b[0x으로 선포하노라!"), kunshu);
				force.kokugou = 국호_진; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x진\x1b[0x으로 선포하였다.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_1();
			}
			else if (n == 4)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x성\x1b[0x으로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x성\x1b[0x으로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x성\x1b[0x으로 선포하노라!"), kunshu);
				force.kokugou = 국호_성; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x성\x1b[0x으로 선포하였다.", kunshuname  )));				
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
				pk::encode("하"),
				pk::encode("상"),
				pk::encode("주"),
				pk::encode("신"),
				pk::encode("연"),
				pk::encode("BACK"),
			};
			
			int n = pk::choose(pk::encode("국호를 선택해주십시오."), kokugou_list);
			
			if (n == 0)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x하\x1b[0x로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x하\x1b[0x로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x하\x1b[0x로 선포하노라!"), kunshu);
				force.kokugou = 국호_하; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x하\x1b[0x로 선포하였다.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_2();
			}
			else if (n == 1)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x상\x1b[0x으로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x상\x1b[0x으로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x상\x1b[0x으로 선포하노라!"), kunshu);
				force.kokugou = 국호_상; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x상\x1b[0x으로 선포하였다.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_2();
			}
			else if (n == 2)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x주\x1b[0x로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x주\x1b[0x로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x주\x1b[0x로 선포하노라!"), kunshu);
				force.kokugou = 국호_주; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x주\x1b[0x로 선포하였다.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_2();
			}
			else if (n == 3)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x신\x1b[0x으로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x신\x1b[0x으로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x신\x1b[0x으로 선포하노라!"), kunshu);
				force.kokugou = 국호_신; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x신\x1b[0x으로 선포하였다.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_2();
			}
			else if (n == 4)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x연\x1b[0x으로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x연\x1b[0x으로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x연\x1b[0x으로 선포하노라!"), kunshu);
				force.kokugou = 국호_연; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x연\x1b[0x으로 선포하였다.", kunshuname  )));					
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
				pk::encode("조"),
				pk::encode("제"),
				pk::encode("기"),
				pk::encode("노"),
				pk::encode("설"),
				pk::encode("BACK"),
			};
			
			int n = pk::choose(pk::encode("국호를 선택해주십시오."), kokugou_list);
			
			if (n == 0)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x조\x1b[0x로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x조\x1b[0x로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x조\x1b[0x로 선포하노라!"), kunshu);
				force.kokugou = 국호_조; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x조\x1b[0x로 선포하였다.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_3();
			}
			else if (n == 1)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x제\x1b[0x로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x제\x1b[0x로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x제\x1b[0x로 선포하노라!"), kunshu);
				force.kokugou = 국호_제; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x제\x1b[0x로 선포하였다.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_3();
			}
			else if (n == 2)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x기\x1b[0x로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x기\x1b[0x로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x기\x1b[0x로 선포하노라!"), kunshu);
				force.kokugou = 국호_기; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x기\x1b[0x로 선포하였다.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_3();
			}
			else if (n == 3)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x노\x1b[0x로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x노\x1b[0x로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x노\x1b[0x로 선포하노라!"), kunshu);
				force.kokugou = 국호_노; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x노\x1b[0x로 선포하였다.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_3();
			}
			else if (n == 4)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x설\x1b[0x로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x설\x1b[0x로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x설\x1b[0x로 선포하노라!"), kunshu);
				force.kokugou = 국호_설; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x설\x1b[0x로 선포하였다.", kunshuname  )));					
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
				pk::encode("서"),
				pk::encode("송"),
				pk::encode("소"),
				pk::encode("정"),
				pk::encode("허"),
				pk::encode("BACK"),
			};
			
			int n = pk::choose(pk::encode("국호를 선택해주십시오."), kokugou_list);
			
			if (n == 0)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x서\x1b[0x로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x서\x1b[0x로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x서\x1b[0x로 선포하노라!"), kunshu);
				force.kokugou = 국호_서; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x서\x1b[0x로 선포하였다.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_4();
			}
			else if (n == 1)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x송\x1b[0x으로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x송\x1b[0x으로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x송\x1b[0x으로 선포하노라!"), kunshu);
				force.kokugou = 국호_송; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x송\x1b[0x으로 선포하였다.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_4();
			}
			else if (n == 2)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x소\x1b[0x로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x소\x1b[0x로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x소\x1b[0x로 선포하노라!"), kunshu);
				force.kokugou = 국호_소; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x소\x1b[0x로 선포하였다.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_4();
			}
			else if (n == 3)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x정\x1b[0x으로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x정\x1b[0x으로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x정\x1b[0x으로 선포하노라!"), kunshu);
				force.kokugou = 국호_정; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x정\x1b[0x으로 선포하였다.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_4();
			}
			else if (n == 4)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x허\x1b[0x로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x허\x1b[0x로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x허\x1b[0x로 선포하노라!"), kunshu);
				force.kokugou = 국호_허; 
		        force.update();
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x허\x1b[0x로 선포하였다.", kunshuname  )));					
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
				pk::encode("우"),
				pk::encode("양"),
				pk::encode("채"),
				pk::encode("당"),
				pk::encode("등"),
				pk::encode("BACK"),
			};
			
			int n = pk::choose(pk::encode("국호를 선택해주십시오."), kokugou_list);
			
			if (n == 0)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x우\x1b[0x로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x우\x1b[0x로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x우\x1b[0x로 선포하노라!"), kunshu);
				force.kokugou = 국호_우; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x우\x1b[0x로 선포하였다.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_5();
			}
			else if (n == 1)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x양\x1b[0x으로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x양\x1b[0x으로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x양\x1b[0x으로 선포하노라!"), kunshu);
				force.kokugou = 국호_양; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x양\x1b[0x으로 선포하였다.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_5();
			}
			else if (n == 2)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x채\x1b[0x로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x채\x1b[0x로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x채\x1b[0x로 선포하노라!"), kunshu);
				force.kokugou = 국호_채; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x채\x1b[0x로 선포하였다.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_5();
			}
			else if (n == 3)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x당\x1b[0x으로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x당\x1b[0x으로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x당\x1b[0x으로 선포하노라!"), kunshu);
				force.kokugou = 국호_당; 
		        force.update();
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x당\x1b[0x으로 선포하였다.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_5();
			}
			else if (n == 4)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x등\x1b[0x으로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x등\x1b[0x으로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x등\x1b[0x으로 선포하노라!"), kunshu);
				force.kokugou = 국호_등; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x등\x1b[0x으로 선포하였다.", kunshuname  )));					
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
				pk::encode("수"),
				pk::encode("초"),
				pk::encode("월"),
				pk::encode("진"),
				pk::encode("조"),
				pk::encode("BACK"),
			};
			
			int n = pk::choose(pk::encode("국호를 선택해주십시오."), kokugou_list);
			
			if (n == 0)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x수\x1b[0x로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x수\x1b[0x로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x수\x1b[0x로 선포하노라!"), kunshu);
				force.kokugou = 국호_수; 
		        force.update();
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x수\x1b[0x로 선포하였다.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_6();
			}
			else if (n == 1)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x초\x1b[0x로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x초\x1b[0x로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x초\x1b[0x로 선포하노라!"), kunshu);
				force.kokugou = 국호_초; 
		        force.update();
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x초\x1b[0x로 선포하였다.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_6();
			}
			else if (n == 2)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x월\x1b[0x로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x월\x1b[0x로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x월\x1b[0x로 선포하노라!"), kunshu);
				force.kokugou = 국호_월; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x월\x1b[0x로 선포하였다.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_6();
			}
			else if (n == 3)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x진\x1b[0x으로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x진\x1b[0x으로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x진\x1b[0x으로 선포하노라!"), kunshu);
				force.kokugou = 국호_진2; 
		        force.update(); 
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x진\x1b[0x으로 선포하였다.", kunshuname  )));					
			    }
		        if (k == 1) 			
				kokugou_6();
			}
			else if (n == 4)
			{
				array<string> answers =
				{
					pk::encode("국호를 \x1b[1x조\x1b[0x로 한다."),
					pk::encode("BACK")
				};
				
				int k = pk::choose(pk::encode("국호를 \x1b[1x조\x1b[0x로 하시겠습니까?"), answers);
		        if (k == 0)
			    {					
				pk::message_box(pk::encode("이 나라의 국호를 \x1b[1x조\x1b[0x로 선포하노라!"), kunshu);
				force.kokugou = 국호_조2; 
		        force.update();
			    pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x(은)는 국호를 \x1b[1x조\x1b[0x로 선포하였다.", kunshuname  )));					
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
        //  왕 즉위 타국 반응
        //----------------------------------------------------------------------------------



		void scene_왕공즉위_타국군주반응()
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
			pk::message_box(pk::encode(pk::format("한편, \x1b[1x{}\x1b[0x의 \x1b[1x{}\x1b[0x 즉위 소식은 다른 군주들에겐 경계심을 더욱 키우는 계기가 되었다.", kunshuname, current_title)));
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
	
		} // scene_왕공즉위_타국군주반응()




        //----------------------------------------------------------------------------------
        //  황제 즉위 타국 반응
        //----------------------------------------------------------------------------------



		void scene_황제즉위_타국군주반응()
		{
			string kunshuname = pk::decode(pk::get_name(kunshu));
		   // 즉위 소식에 분개할 타국 군주 대사
            if (pk::is_alive(enemy_kunshu) and pk::is_alive(enemy_force_1)) 	
		    {
             if (enemy_kunshu.get_id() == kunshu.get_id()) 
                 return;
				
			pk::move_screen(enemy_building.get_pos());
            pk::play_bgm(20);
			
            string emperor_title = pk::decode(pk::get_name(pk::get_title(작위_황제)));			
			pk::message_box(pk::encode(pk::format("한편, \x1b[1x{}\x1b[0x의 \x1b[1x{}\x1b[0x 취임에 분개한 곳도 있었으니....", kunshuname, emperor_title)));
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x.... 이 놈이 감히 나를 신하 취급해? 이 일을 내가 가만히 보고 있을 성 싶으냐!" , kunshuname)), enemy_kunshu);

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

		   // 즉위 소식에 분개할 한실 중시 군주 대사
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
                  if (pro_han_force.get_id() != force.get_id() and pro_han_kunshu_group.kanshitsu == 한실_중시) 	
			      {			
                   pk::play_bgm(20);														
		           pk::building@ pro_han_kunshu_group_building = pk::get_building(pro_han_kunshu_group.service);																						 
			       pk::move_screen(pro_han_kunshu_group_building.get_pos());																											 
			       pk::message_box(pk::encode(pk::format("한조의 배신자! 내 \x1b[1x{}\x1b[0x 이 역적놈을 가만 두지 않으리!" , kunshuname)), pro_han_kunshu_group);
                   pk::add_relations(pro_han_force, force.get_id(), -타국군주_우호도하락량 );
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

                   pk::message_box(pk::encode(pk::format("여기에 한조를 중시했던 \x1b[2x{}\x1b[0x - \x1b[2x{}\x1b[0x명의 군주(들)의 \x1b[1x{}\x1b[0x에 대한 우호도가 추가로 하락했다.", person_names, pro_han_force_group_list.count, kunshuname)));
 	
				   pk::fade(0);
				   pk::sleep();
				   pk::background(-1);
				   pk::fade(255);	

			    }			  
		    }		

		} // scene_황제즉위_타국군주반응()



		void scene_황제처형_타국군주반응()
		{
			string kunshuname = pk::decode(pk::get_name(kunshu));
		   // 즉위와 처형 소식에 분개할 타국 군주 대사
          if (pk::is_alive(enemy_kunshu) and pk::is_alive(enemy_force_1)) 	
		  {
             if (enemy_kunshu.get_id() == kunshu.get_id()) 
                 return;
							
            if (emperor.mibun == 신분_사망)									   								
		    {
			pk::move_screen(enemy_building.get_pos());
            pk::play_bgm(20);			
			pk::message_box(pk::encode(pk::format("한편, \x1b[1x{}\x1b[0x가 황제를 처형하고 제위에 올랐다는 소식에 분개한 곳도 있었으니....", kunshuname)));
			pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x.... 이 극악무도한 놈! 기어코 황제를 죽이고 야욕을 드러냈구나! 이제 천하가 우습게 보인다는 것이냐! 허나 나를 업신여겼다간 큰 화를 면치 못할 것이다!" , kunshuname)), enemy_kunshu);

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
               pk::add_relations(enemy_force_1, force.get_id(), -타국군주_우호도하락량_황제처형 );				
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
		  }

		   // 즉위와 처형 소식에 분개할 한실 중시 군주 대사
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
               if (pro_han_force.get_id() != force.get_id() and pro_han_kunshu_group.kanshitsu == 한실_중시) 	
	           {		         										  
                     if (emperor.mibun == 신분_사망)									   								
		             {																									 
                     pk::play_bgm(20);														
		             pk::building@ pro_han_kunshu_group_building = pk::get_building(pro_han_kunshu_group.service);																						 
			         pk::move_screen(pro_han_kunshu_group_building.get_pos());																											 
			         pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x 이 역적놈! 한조를 멸망시킨 것도 모자라 황제를 죽이는 천인공노할 짓을 저지르다니! 이 원수는 절대 잊지 못할 것이다!" , kunshuname)), pro_han_kunshu_group);
                     pk::add_relations(pro_han_force, force.get_id(), -타국군주_우호도하락량_황제처형 );		                               
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

                   pk::message_box(pk::encode(pk::format("여기에 한조를 중시했던 \x1b[2x{}\x1b[0x - \x1b[2x{}\x1b[0x명의 군주(들)의 \x1b[1x{}\x1b[0x에 대한 우호도가 추가로 하락했다.", person_names, pro_han_force_group_list.count, kunshuname)));
          							
						pk::fade(0);
						pk::sleep();
						pk::background(-1);
						pk::fade(255);
			    }						
			
		  }		

		} // scene_황제처형_타국군주반응()



        //----------------------------------------------------------------------------------
        //  한실 주의자들의 플레이어 혐오 무장 추가
        //----------------------------------------------------------------------------------



		// 한실주의자들의 혐오무장에 추가
		void scene_한실주의자의_혐오()
		{
			string kunshuname = pk::decode(pk::get_name(kunshu));
		    pk::person@ hansilman_group_1; 
		    pk::building@ hansilman_group_1_building;
			
            pk::list<pk::person@> hansilman_list;
            hansilman_list.clear();			
			for (int i = 0; i < 무장_끝; i++)
			{
                pk::person@ hansilman = pk::get_person(i);

				if (pk::is_alive(hansilman) and hansilman.get_id() != kunshu.get_id())
				{	
				  if (hansilman.kanshitsu == 한실_중시 and !pk::is_dislike(hansilman, kunshu.get_id())
					  and pk::get_num_dislikes(hansilman) < 5 and hansilman.mibun != 신분_사망
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

            pk::message_box(pk::encode(pk::format("\x1b[1x{}\x1b[0x, 이 역적놈! 이 천륜을 저버린 죄를 기필코 징벌하리라!" , kunshuname)), hansilman_group_1);																						 
		
			string hansilman_group_1_name = pk::decode(pk::get_name(hansilman_group_1));
 
            pk::message_box(pk::encode(pk::format("\x1b[2x{}\x1b[0x(을)를 포함한 \x1b[2x{}\x1b[0x명의 각계의 한실 중시 인사들이 \x1b[1x{}\x1b[0x를 혐오하기 시작했다.", hansilman_group_1_name, hansilman_list.count, kunshuname)));
	 
			pk::fade(0);
			pk::sleep();
			pk::background(-1);
			pk::fade(255);	


		} // scene_한실주의자의_혐오()



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