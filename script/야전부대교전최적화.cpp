/*
// ������: �⸶å��
// Update: '19.2.1     / ���ʳ���: ���������δ� ������ ���ݸ�ǥ�δ� �������ϴ� ��ũ��Ʈ �ۼ� 
// Update: '19.2.19    / ���泻��: ����ġ�ɼ�(����,�ź�), �Ǽ����� ���ܿɼ�, �跫�δ� ���ܿɼ�, ������ ������� �ɼ� �ݿ�
// Update: '19.2.27    / ���泻��: NPC�δ� �������� �߰�
// Update: '19.3.1     / ���泻��: �δ��ǥ����_���°���ġ���� �ɼ� �߰�
// Update: '19.3.4     / ���泻��: null point error ���� ����
// Update: '19.3.9     / ���泻��: �˸����� �δ� �ҼӰ������� ���� ��� �߰�
// Update: '19.3.25    / ���泻��: �ֺ� ���� ���� �� �������� ���� �̵�
// Update: '20.8.26    / ���泻��: ķ���ο����� ��ũ��Ʈ ȿ�� �ߵ����� �ʵ��� ����, ���̵�_Ư�޿����� �����ϵ��� ����
// Update: '20.9.8     / ���泻��: �跫Ư�� ���δ� ���߰����� ���� ����ġ ���� �߰�, ������������ �������� �߰�, �ӹ� ������Ʈ ��������(�˻����ҽ� ����)
// Update: '20.9.16    / ���泻��: ����������/�Ʊ������������� �˰��� ���� �߰�
// Update: '20.9.19    / ���泻��: �Ʊ������������� �˰��� ����, ���Ժδ�_�ʰ����±�ȯ ��� �߰�, �������� �ĺ��δ��� �������� ���� �߰�
// Update: '20.9.21    / ���泻��: ���Ժδ�_�ʰ����±�ȯ �����ʰ� Ȯ������ �߰�, ��ǥ���� ���信 ���� ��쿡�� �ʰ����� Ȯ�� (�������� �������)
*/

namespace �����δ뱳������ȭ
{
    //=======================================================================================
    // ���� ���� (true = on, false = off)
    
    // �δ�/���� ���ݸ�ǥ ���� �ñ� ���� (3: �Ͻ���/�߰� ���, 2: ���߰� �������Ǹ���, 1: �Ͻ��� �ÿ���,  0: ���OFF)
    const int  ������Ʈ_�ֱ⼳�� = 3;       // ���� : ������Ʈ �ֱ⸦ 2 �Ǵ� 3���� ���� �� ������ �������� ������ ������ �� ����
    
    const bool ��ǥ�δ뺯��     = true;    // ���ݸ�ǥ�δ� �缳�� �� Ÿ�Ϻδ� �������� (������ǥ ���濡�� ���� ����)
    const bool ��ȭǥ�ü���     = true;    // ���ݸ�ǥ ���泻�� ��ȭǥ�ÿ���
    
    const int  �δ��ǥ����_�������� = 0;     // 0: ��ǻ��AI �� �÷��̾�_���ӱ��� ���,  1: �÷��̾�_���ӱ��ܸ�, 2: ��ǻ��AI��,  3: ��� ������
    const bool �δ��ǥ����_������������ = false;   // �������ǰ� ������ ���뿩�� ����, ������ �ӹ����� ���õǹǷ� false ����
    
    // ����ġ �߰��ɼ� ���� (�⺻�����δ� ����/���������� �Ǵ���)
    const bool �δ��ǥ����_���ݰ���ġ���� = false;   // ��ǥ����� �δ���ݷ��� ����ġ �߰� ����
    const bool �δ��ǥ����_����ġ���� = true;   // ��ǥ����� �δ������ ����ġ �߰� ����
    const bool �δ��ǥ����_�źа���ġ���� = true;   // ��ǥ����� ����/���� �δ��� ��� ����ġ �߰� ���� (�нú�ȿ��.cpp �� �δ�˸�ȿ�� ��� �� ��õ ����)
    const bool �δ��ǥ����_���°���ġ���� = true;   // ��ǥ���� �ӹ��δ��� �������̿� ���� ����ġ �߰� ���� (�˸��Ű�� ���� �Һδ븦 ������ ���ɼ� ����)
    const bool �δ��ǥ����_�跫����ġ���� = false;   // ��ǥ����� �跫Ư�� �����δ��� ��� ����ġ �߰� ����
    
    // �������� �� �Ǽ����� �Ǵ� ����
    const bool �Ǽ�����_���ܼ���    = true;
    const int  �Ǽ�����_���ؼ�����  = 1000;     // ���ذ��� �������� ��ǻ�� �Ǽ���ɺҰ�
    const int  �Ǽ�����_���غ���    = 3000;     // ���� ������ �̻� �� ���غ��� ������ ��� �Ǽ������� ���
    const bool �Ǽ�����_�ӹ����    = true;     // �δ��ӹ��� ��ġ�� ������ ��� ���� �� �ü� ���
    
    // �������� �� �跫�δ� �Ǵ� ����
    const bool  �跫�δ�_���ܼ���  = true;
    const bool  �跫�δ�_Ư���� = true;     // �跫�迭 Ư�� ������ �δ� ��� (�Ż�,���,����,�͸�,��ȯ�� ������ ���)
    const float �跫�δ�_���°���� = 1.5f;      // �δ������� �δ������� ��� ���� �̻��̸� �跫�δ�� ���
    
    // �������� �� ���/����/���� �δ� �δ����� ���� ('20.8.30)
    const bool  ��������_�δ�����_��������  = true; // ��������δ밡 �δ밣 ������ �������� ����
    const int   ��������_�δ�����_���ر��  = 60;   // ��������δ밡 �δ밣 ������ �������� (��� �̻��� ��� ����)
    
    //---------------------------------------------------------------------------------------
    
    // ���� ���� �ֺ� ���� ���� ���� �� ����
    const bool �Ʊ�����_�������� = true;   // ���� ���� �δ� �ֺ��� ���� ���� ������ ���� ���� �� ���� �����Ͽ� ���� ����
    const bool ���ð���_�������� = true;   // �Ʊ�����_�������� ���� true �� ���: ���ð��� �������� ���� Ȱ��ȭ
    const bool �����ױ�_�������� = true;   // �Ʊ�����_�������� ���� true �� ���: �����ױ� �������� ���� Ȱ��ȭ
    
    // ���� ħ�� �δ��� �������� ���
    const bool ������_�������    = true;   // �δ밡 ������ ���� �� �ֺ��� ���������� �δ밡 ���� ��� ���� �������� �ӹ�������
    const bool ������_���ɼ������� = true;   // ���� ��� �� �δ� ������ ���ݿ� ���� �ൿ (true=���,������ ���ɽ���  / false=���� ������� ���ɽ���)
    
    // ���� ���� �� ���� �ʰ��� ��� �ҼӰ������� ���� 
    const bool  ���Ժδ�_�ʰ����±�ȯ = true;   // �������� �δ����� �� �����ѵ� �ʰ����� Ȯ�� �� ��ȯ��ǥ �缳��
    const float ���Ժδ�_�ʰ��ѵ����� = 1.0f;   // �����ִ뺴�� ��� ����/����(����+�δ�) �ѵ��ʰ� ���ؼ��� (0.0f~1.0f)
    const bool  ���Ժδ�_�ʰ������ȯ = true;   // �������� �δ����� �� �����ѵ� �ʰ����� Ȯ�� �� ��ȯ��ǥ �缳��
    const float ���Ժδ�_�ʰ�����_�������±��� = 0.5f;   // �����ִ뺴�� ��� ���纴���� ���� ������ ��� ���� �ʰ� ���� (0.0f~1.0f)
    
    //---------------------------------------------------------------------------------------
    const bool �ؼҺ��ºδ�_����������� = true;    // �˸����� AI�δ� �ҼӰ������� ���� ��� (�����δ� ������)
    const int  �ؼҺ��ºδ�_���ͺ��±��� = 500;     // �ҼӰ��� ���� ���� ����, ���� �̸� �� ���� ������
    
    //=======================================================================================
    
    
	class Main
	{
        
        Main()
		{
            pk::bind(102, pk::trigger102_t(onGameInit));
            pk::bind(111, pk::trigger111_t(onTurnStart));
            
            pk::bind(160, pk::trigger160_t(onBuildingCreate));
            pk::bind(161, pk::trigger161_t(onBuildingDestroyed));

            pk::bind(170, pk::trigger170_t(onUnitCreate));
            pk::bind(171, pk::trigger171_t(onUnitEliminated));
            pk::bind(172, pk::trigger172_t(onUnitPosChange));
            pk::bind(173, pk::trigger173_t(onUnitExpAdd));
        }
        //---------------------------------------------------------------------------
        
        pk::unit@ src_unit;
        pk::unit@ dst_unit_old;
        pk::unit@ dst_unit_new;
        
        pk::list<pk::unit@> list_candidate_unit;
        array<int> arr_target_unit_id(�δ�_��, -1);
        array<int> arr_target_base_id(�δ�_��, -1);
        
        array<float> heishu_weight(6, 0.0f);
        float heishu_null   = 0.0f;   
        float heishu_weak   = 0.5f;   
        float heishu_normal = 1.0f;   
        float heishu_strong = 1.5f;   
        int order_update_range = 10;  // �ǹ� ����/���� �Ǵ� ���� ����/����/�̵� �� �ӹ�������Ʈ ���δ� �˻����� ('20.9.7)
        
        //---------------------------------------------------------------------------
        // Ʈ���� ���ۺ�
        //---------------------------------------------------------------------------
        
        // ���� �ʱ�ȭ �� ������Ʈ
        void onGameInit()
        {
            if (pk::is_campaign()) return ;
            if (pk::get_scenario().no >= 16) return;
            if (pk::get_scenario().difficulty < ���̵�_Ư��) return;
            clear_target_info();
            pk::force@ force = pk::get_force(pk::get_current_turn_force_id());
            onTurnStart(force);
        }
        
        // �� ���� �� ������Ʈ
        void onTurnStart(pk::force@ force)
        {
            if (pk::is_campaign()) return;
            if (pk::get_scenario().difficulty < ���̵�_Ư��) return;
            
            if (!pk::is_normal_force(force)) return;
            optimize_current_force_target();    // �����Լ�ȣ��
        }
        
        // �ǹ� ���� �� ������Ʈ
        void onBuildingCreate(pk::building@ building, int type)
        {
            if (pk::is_campaign()) return;
            if (pk::get_scenario().difficulty < ���̵�_Ư��) return;
            
            optimize_pos_range_target(building.pos);    // �����Լ�ȣ�� ('20.9.7)
        }
        
        // �ǹ� �ı� �� ������Ʈ
        void onBuildingDestroyed(pk::building@ building, int type)
        {
            if (pk::is_campaign()) return;
            if (pk::get_scenario().difficulty < ���̵�_Ư��) return;
            
            optimize_pos_range_target(building.pos);    // �����Լ�ȣ�� ('20.9.7)
        }
        
        // �δ� ���� �� ������Ʈ
        void onUnitCreate(pk::unit@ unit, int type)
        {
            if (pk::is_campaign()) return;
            if (pk::get_scenario().difficulty < ���̵�_Ư��) return;
            
            // ������ �δ��� ���ݸ�ǥ�� �ʱ�ȭ
            arr_target_unit_id[unit.get_id()] = -1;
            arr_target_base_id[unit.get_id()] = -1;
        }
        
        // �δ� �˸� �� ������Ʈ
        void onUnitEliminated(pk::unit@ unit, int type)
        {
            if (pk::is_campaign()) return;
            if (pk::get_scenario().difficulty < ���̵�_Ư��) return;
            
            // �˸�� �δ��� ���ݸ�ǥ�� �ʱ�ȭ
            arr_target_unit_id[unit.get_id()] = -1;
            arr_target_base_id[unit.get_id()] = -1;
            
            optimize_pos_range_target(unit.pos);    // �����Լ�ȣ�� ('20.9.7)
        }
        
        // �δ� ����ġ ȹ�� ��
        void onUnitExpAdd(pk::unit@ unit, int type)
        {
            if (pk::is_campaign()) return;
            if (pk::get_scenario().difficulty < ���̵�_Ư��) return;
            if (!pk::is_alive(unit)) return;

            optimize_pos_range_target(unit.pos);    // �����Լ�ȣ�� ('20.9.7)
        }
        
        // �δ� ��ġ ���� �� (������ ���� �̵� ����)
        void onUnitPosChange(pk::unit@ unit_t, const pk::point &in pos)
        {
            if (pk::is_campaign()) return;
            if (pk::get_scenario().difficulty < ���̵�_Ư��) return;
            
            if (!check_valid_unit(unit_t)) return;
            
            int force_id = pk::get_current_turn_force_id();
            if (!pk::is_normal_force(force_id)) return;
            pk::force@ force = pk::get_force(force_id);
            
            optimize_pos_range_target(pos);    // �����Լ�ȣ�� ('20.9.7)
            
        }
        
        //---------------------------------------------------------------------------
        // ���� ��ǥ�κ��� ���� �Ÿ� ���� �ִ� ���� �� ���� �δ� ����ȭ (�����Լ� �߰�, '20.9.7)
        void optimize_pos_range_target(pk::point pos_t)
        {
            if (������Ʈ_�ֱ⼳�� != 2 and ������Ʈ_�ֱ⼳�� != 3) return;
            
            if (!pk::is_valid_pos(pos_t)) return;
            
            int force_id = pk::get_current_turn_force_id();
            if (!pk::is_normal_force(force_id)) return;
            pk::force@ force = pk::get_force(force_id);
            
            // �ش� ��ǥ�κ��� ���� �� ���� �� �δ�˻�
            pk::array<pk::point> arr_range = pk::range(pos_t, 1, order_update_range);
            for (int i = 0; i < int(arr_range.length); i++)
            {
                pk::unit@ unit = pk::get_unit(arr_range[i]);
                if (check_valid_unit(unit) and check_force_option(unit))
                {
                    if (!unit.action_done and unit.get_force_id() == force_id )
                        optimize_combat_unit_target(unit);     // �δ��ǥ �缳�� �Լ�    
                }
            }
        }
        
        // ���� �� ���� �δ� ����ȭ (�����Լ�)
        void optimize_current_force_target()
        {
            if (������Ʈ_�ֱ⼳�� != 1 and ������Ʈ_�ֱ⼳�� != 3) return;
            
            int force_id = pk::get_current_turn_force_id();
            if (!pk::is_normal_force(force_id)) return;
            pk::force@ force = pk::get_force(force_id);
            
            // ���� ��ü ���ൿ �δ� 
            pk::array<pk::unit@> arr_unit_list = pk::list_to_array(pk::get_unit_list());
            if (int(arr_unit_list.length) == 0) return;
            for (int i = 0; i < int(arr_unit_list.length); i++)
            {
                pk::unit@ unit = arr_unit_list[i];
                if (check_valid_unit(unit) and check_force_option(unit))
                {
                    if (!unit.action_done and unit.get_force_id() == force_id )
                        optimize_combat_unit_target(unit);     // �δ��ǥ �缳�� �Լ�    
                }
            }
        }
        
        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------
        
        // �δ��ǥ �缳�� �Լ�
        void optimize_combat_unit_target(pk::unit@ unit)
        {
            // ����δ밡 �ƴϸ� ����
            if (!pk::is_alive(unit)) return;
            if (unit.status != �δ����_���) return;
            
            // �δ� ��ǥ
            @dst_unit_old = pk::hex_object_to_unit(pk::get_hex_object(unit.target_pos));
            
            // ------ ���� ������ ���δ� �˻��Լ� ȣ�� -------------
            // ���� :: def_force_base_nearby, atk_enemy_base_nearby, retreat_exceed_unit �Լ����� ���� ����� ��
            get_list_candidate_unit(unit);
            int candidate_count = list_candidate_unit.count;
            if (candidate_count == 0)   // �δ밡 ���������� ��� �����ֺ� �߰�Ž��
            {
                search_enemy_on_reinforce_engage(unit);
                candidate_count = list_candidate_unit.count;
            }// --------------------------------------------
            
            // ������ ������ �α� ���
            if (����׸��)  
            {
                string src_name = (pk::is_alive(unit))?         pk::decode(pk::get_name(pk::get_person(unit.leader)))+"��"         : "Ȯ�κҰ�" ;
                string dst_name = (pk::is_alive(dst_unit_old))? pk::decode(pk::get_name(pk::get_person(dst_unit_old.leader)))+"��" : "Ȯ�κҰ�" ;
                pk::info( pk::format("{} ��ó�δ��={}", src_name, candidate_count) );
            }

            // ��������, ��������, ���ɱ� �ʰ����±�ȯ
            if (�Ʊ�����_�������� and def_force_base_nearby(unit)) return;  // �ֺ��� ���� ���� �Ʊ� ���� ����
            if (������_�������  and atk_enemy_base_nearby(unit)) return;  // �δ���ǥ�� ���/���� ���� �Ǵ� : ���������̸� �������� ����
            if ((���Ժδ�_�ʰ����±�ȯ or ���Ժδ�_�ʰ������ȯ) and retreat_exceed_unit(unit)) return;  // ���� ����/���� �ʰ� �� Ÿ�������� ���κ���
            
            // �˸����� �δ� ���� ���� ('19.3.9)
            if (�ؼҺ��ºδ�_����������� and retreat_battlefield(unit)) return;

            // AI�Ǽ������� ��� �ӹ� �̺���
            if (�Ǽ�����_���ܼ��� and (check_construction_unit(unit))) return ;
            
            // �跫�δ��� ��� �ӹ� �̺���
            if (�跫�δ�_���ܼ��� and (func_strategy_unit_type(unit) > 0)) return ;
            
            // ���ݸ�ǥ�δ� ����
            if (��ǥ�δ뺯�� and candidate_count > 0)
                func_change_target_unit(unit);  // �ֺ� ���δ� ���ݼ���

        }
        
        
        //---------------------------------------------------------------------------
        
        // �ؼҺ��� �δ� ���� ��Ż ���
        bool retreat_battlefield(pk::unit@ unit)
        {
            if (!pk::is_alive(unit)) return false;
            if (pk::is_player_controlled(unit)) return false;
            
            if (int(unit.troops) < �ؼҺ��ºδ�_���ͺ��±���)
            {
                int unit_id = unit.get_id();
                int service_id = pk::get_service(unit);
                pk::building@ base = pk::get_building(pk::get_service(unit));
                if (pk::is_alive(base) and arr_target_base_id[unit_id] != service_id and pk::get_distance(base.pos, unit.pos) <= 25)
                {
                    arr_target_base_id[unit_id] = service_id;
                    pk::set_order(unit, �δ��ӹ�_�̵�, base.pos);
                    return true;
                }
            }
            
            return false;
        }
        
        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------
        
        // ���� ������ ���δ� �˻��Լ�
        void get_list_candidate_unit(pk::unit@ unit)
        {
            // ����Ʈ �ʱ�ȭ
            if (!pk::is_alive(unit)) return;
            list_candidate_unit.clear();
            
            int weapon_id = unit.weapon;
            int weapon_type = get_weapon_type(weapon_id);   // 0:��, 1:����, 2:����, 3:������Ÿ, 4:����
            
            // ������ �˻���󿡼� ����
            if (weapon_id == ����_����) return;   
            // ���/����/���� �˻���󿡼� ���ܿ���
            if (!��������_�δ�����_�������� and weapon_type == 3) return;
            if ( ��������_�δ�����_�������� and weapon_type == 3 and (unit.energy < ��������_�δ�����_���ر��)) return;
            
            // ���� ��Ÿ� ���� --- �Լ� ���� ('20.9.13)
            pk::int_int atk_range = get_atk_range(unit);
            int min = (weapon_id == ����_����)? 0 : atk_range.first;
            int max = (weapon_id == ����_����)? 0 : atk_range.second;
            
            pk::array<pk::point> range = pk::get_movable_pos(unit);  // �̵�������ǥ
            for (int i = 0; i < int(range.length); i++)
			{
                pk::point pos = range[i];
                pk::hex@ hex = pk::get_hex(pos);
                pk::array<pk::point> arr_t = pk::range(pos, min, max);   // �ֺ���ǥ
                for (int j = 0; j < int(arr_t.length); j++)
                {
                    pk::unit@ target_unit = pk::get_unit(arr_t[j]);
                    if (!hex.has_building and !hex.has_unit and pk::is_alive(target_unit) 
                        and pk::is_enemy(unit, target_unit) and !list_candidate_unit.contains(target_unit))
                    {
                        list_candidate_unit.add(target_unit);   // ���δ� ���� ���� �� ����Ʈ �߰�
                    }
                }
			}
        }
        
        // �δ��ӹ��� �ڼ��� �������� �̵��� ���, �����ֺ��� ���δ� �ִ��� �߰� Ȯ��
        void search_enemy_on_reinforce_engage(pk::unit@ unit)
        {
            if (!pk::is_alive(unit)) return;
            if (list_candidate_unit.count > 0) return;
            if (unit.type != �δ�����_����) return;
            if (unit.weapon > ����_����) return;
            
            pk::building@ service_base = pk::get_building(pk::get_service(unit));
            if (!pk::is_alive(service_base)) return;
            
            pk::building@ base_p = pk::get_building(pk::get_building_id(unit.pos));
            if (!pk::is_alive(base_p)) return;
            
            int service_id = service_base.get_id();
            int base_p_id = base_p.get_id();
            if (base_p_id == service_id or unit.get_force_id() != base_p.get_force_id()) return;    // �ҼӰ��� �����̰ų� Ÿ���� ������ ���
            
            if (unit.target_type == �δ��ӹ����_����)
            {
                pk::building@ base_t = pk::get_building(unit.target);
                if (pk::is_alive(base_t))
                {
                    int base_t_id = base_t.get_id();
                    // ��ǥ������ �δ���ǥ������ ���ų�, �δ� �̵��Ÿ� ���� ��ǥ������ �����ϴ� ���
                    if (base_t_id < �ǹ�_������ and (base_t_id == base_p_id or check_base_in_movable_range(unit, base_t)))
                    {
                        pk::array<pk::point> range = pk::range(base_p.pos, 1, 12);  // ���� �ֺ� �˻�
                        for (int i = 0; i < int(range.length); i++)
                        {
                            pk::point pos_i = range[i];
                            pk::unit@ unit_i = pk::get_unit(pos_i);
                            if (pk::is_alive(unit_i) and pk::get_building_id(pos_i) == base_p_id
                                and pk::is_enemy(unit, unit_i) and !list_candidate_unit.contains(unit_i))
                            {
                                list_candidate_unit.add(unit_i);   // ���δ� ���� ���� �� ����Ʈ �߰�
                            }
                        }
                    }
                }
            }
        }
        
        
        // ���ݸ�ǥ�δ� ���� �Լ�
        void func_change_target_unit(pk::unit@ unit)
        {
            if (!pk::is_alive(unit)) return;
            
            @src_unit = @unit;
            int target_count = list_candidate_unit.count;   // �ĺ� �δ��
            if (target_count == 0) return;

            // ��������
            int weapon_id = src_unit.weapon;
            int weapon_type = get_weapon_type(weapon_id);   // 0:��, 1:����, 2:����, 3:������Ÿ, 4:����

            // �ӹ����� ���δ�
            bool combat_update = false;
            if      (weapon_type == 1 or weapon_type >= 4) combat_update = true;
            else if (weapon_type == 3 and ��������_�δ�����_�������� and (unit.energy >= ��������_�δ�����_���ر��)) combat_update = true;
            else combat_update = false;
            
            if (combat_update)
            {
                // ������ ����
                set_heishu_weight(weapon_id);
                
                // ������, ����, ���� ����ġ ������ ��ǥ�δ� �ɷ�ġ ����
                list_candidate_unit.sort(function(a, b)
                {
                    float stat_a = main.get_attr_weight(a);
                    float stat_b = main.get_attr_weight(b);
                    return (stat_a > stat_b);
                });
                
                // ��ǥ�δ� ����
                pk::array<pk::unit@> arr_candidate_unit = pk::list_to_array(list_candidate_unit);
                @dst_unit_new = arr_candidate_unit[0];
                
                // ������ ������ �α� ���
                if (����׸��)
                {
                    string src_name = (pk::is_alive(unit))?         pk::decode(pk::get_name(pk::get_person(unit.leader)))+"��"         : "Ȯ�κҰ�" ;
                    string dst_name = (pk::is_alive(dst_unit_new))? pk::decode(pk::get_name(pk::get_person(dst_unit_new.leader)))+"��" : "Ȯ�κҰ�" ;
                    pk::info( pk::format("{} �ӹ���ǥ:{}", src_name, dst_name) );
                }
                
                if (pk::is_alive(dst_unit_new))
                {
                    int src_id = src_unit.get_id();
                    int dst_id = dst_unit_new.get_id();
                    
                    // �� ��ǥ�δ�� ����� ��� �����ӹ� �缳��
                    if (arr_target_unit_id[src_id] != dst_id)
                    {
                        // ��ǥ �缳�� ���� ����
                        arr_target_unit_id[src_id] = dst_id;
                        arr_target_base_id[src_id] = -1;
                        
                        // �����ӹ� �缳��
                        int old_order = src_unit.order;
                        if (old_order == �δ��ӹ�_���� or old_order == �δ��ӹ�_����)
                            pk::set_order(src_unit, old_order, dst_unit_new.pos);
                        else
                            pk::set_order(src_unit, �δ��ӹ�_����, dst_unit_new.pos);
                        
                        // �δ��ӹ����� ��ȭ ǥ��
                        if (��ȭǥ�ü��� and pk::is_in_screen(src_unit.pos))  
                            say_change_target_unit();
                    }
                }
            }
        }

        
        void say_change_target_unit()
        {
            string str_f0 = pk::decode(pk::get_name(pk::get_force(src_unit.get_force_id())));
            string str_p0 = pk::decode(pk::get_name(pk::get_person(src_unit.leader)));
            string str_w0 = get_weapon_name(src_unit.weapon);
            string str_t0 = get_tekisei_name(pk::get_tekisei(src_unit));
            //string str_f1 = pk::decode(pk::get_name(pk::get_force(dst_unit_old.get_force_id())));
            //string str_p1 = pk::decode(pk::get_name(pk::get_person(dst_unit_old.leader)));
            //string str_w1 = get_weapon_name(dst_unit_old.weapon);
            //string str_t1 = get_tekisei_name(pk::get_tekisei(dst_unit_old));
            string str_f2 = pk::decode(pk::get_name(pk::get_force(dst_unit_new.get_force_id())));
            string str_p2 = pk::decode(pk::get_name(pk::get_person(dst_unit_new.leader)));
            string str_w2 = get_weapon_name(dst_unit_new.weapon);
            string str_t2 = get_tekisei_name(pk::get_tekisei(dst_unit_new));
            string str_color = get_heishu_color(dst_unit_new.weapon);
            
            string str_line0 = pk::format("\x1b[2x{}�� \x1b[1x{}�� \x1b[17x{}{}\x1b[0x �ӹ�����!!", str_f0, str_p0, str_w0, str_t0);
            string str_line1 = pk::format("�� \x1b[2x{}�� \x1b[1x{}�� {}{}{}\x1b[0x ����", str_f2, str_p2, str_color, str_w2, str_t2);
            
            pk::person@ leader = pk::get_person(src_unit.leader);
            pk::say(pk::encode(pk::format("{}\n{}", str_line0, str_line1)), leader);
        }
        
        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------
        
        // ----- ������_�������  
        // ���ݺδ� : ���� ��ǥ������ ���� ���� Ȯ�� : true=���� �Ǵ� ���� / false=��� �Ǵ�
        bool atk_enemy_base_nearby(pk::unit@ unit)
        {
            if (!pk::is_alive(unit)) return false;
            if (unit.type != �δ�����_����) return false;
            
            int candidate_count = list_candidate_unit.count;
            if (candidate_count > 4) return false;
            
            pk::building@ service_base = pk::get_building(pk::get_service(unit));
            pk::building@ target_base = pk::get_building(pk::get_building_id(unit.pos));
            if (pk::is_alive(service_base) and pk::is_alive(target_base))
            {
                int unit_id    = unit.get_id();
                int service_id = service_base.get_id();
                int target_id  = target_base.get_id();
                pk::person@ leader = pk::get_person(unit.leader);
                
                // �� ���� ���� (���� ���� ���� �ֺ��� �δ� ���� �ÿ��� ����, ���� ���� �ÿ��� �̵���)
                bool is_enemy_base = pk::is_enemy(unit, target_base);
                bool is_candidate_base = (candidate_count == 0 or (is_weak_base(target_base) and candidate_count > 0));
                if (is_enemy_base and is_candidate_base and (arr_target_base_id[unit_id] != target_id and arr_target_unit_id[unit_id] != -1) )
                {
                    if ((������_���ɼ������� and (leader.character == ����_��� or leader.character == ����_����)) or !������_���ɼ�������)
                    {
                        // ��ǥ �缳�� ���� ����
                        arr_target_base_id[unit_id] = target_id;
                        // �����̸� ������ ���� �缳��
                        pk::set_order(unit, �δ��ӹ�_����, target_base.pos);
                        // ��ǥ���� �޽���
                        if (��ȭǥ�ü��� and pk::is_in_screen(unit.pos))
                        {
                            string target_name = pk::decode(pk::get_name(target_base));
                            switch(pk::rand(3))
                            {
                                case 0: pk::say(pk::encode(pk::format("\x1b[2x{}\x1b[0x��(��) �����϶�", target_name)), leader); break;
                                case 1: pk::say(pk::encode(pk::format("�츮�� \x1b[2x{}\x1b[0x��(��) �����Ѵ�", target_name)), leader); break;
                                case 2: pk::say(pk::encode(pk::format("\x1b[2x{}\x1b[0x(��)�� ���϶�", target_name)), leader); break;
                            }
                        }
                        return true;     
                    }
                }
            }
            return false;            
        }
        
        
        //---------------------------------------------------------------------------
        // ----- �Ʊ�����_�������� 
        // ���δ� : �ֺ��� ���º����� ������������ �̵�
        bool def_force_base_nearby(pk::unit@ unit)
        {
            if (!pk::is_alive(unit)) return false;
            if (unit.type != �δ�����_����) return false;    // ���� ���� �ƴϸ� ����
            
            pk::building@ service = pk::get_building(pk::get_service(unit));
            if (!pk::is_alive(service)) return false;
            
            int candidate_count = list_candidate_unit.count;
            if (candidate_count > 1) return false;
            
            // �ֺ��� ��ǥ���� �˻�
            pk::list<pk::building@> list_target_base;
            pk::array<pk::point> arr = pk::get_movable_pos(unit);
            for (int i = 0; i < int(arr.length); i++)
            {
                pk::building@ base = pk::get_building(arr[i]);
                if (pk::is_alive(base))
                {
                    int base_id = base.get_id();
                    bool is_force_base = (unit.get_force_id() == base.get_force_id());
                    bool is_force_city = (���ð���_�������� and is_force_base and (�ǹ�_���ý��� <= base_id and base_id < �ǹ�_���ó�));
                    bool is_force_gate = (�����ױ�_�������� and is_force_base and (�ǹ�_�������� <= base_id and base_id < �ǹ�_�ױ���));
                    if (is_force_base and pk::enemies_around(base))
                    {
                        if (is_force_gate and is_weak_base(base)) list_target_base.add(base);
                        if (is_force_city and is_weak_base(base)) list_target_base.add(base);
                    }
                }
            }
            
            if (list_target_base.count == 0) return false;
            
            // ��ǥ ������ �ټ��� ��� �켱���� ���� (���ð� �����ױ����� �켱, ���� ���� ���� �켱, ���� ��� �� ������ ���� ���� �켱
            list_target_base.sort(function(a, b)
            {
                bool city_a = (�ǹ�_���ý��� <= a.get_id() and a.get_id() < �ǹ�_���ó�);
                bool city_b = (�ǹ�_���ý��� <= b.get_id() and b.get_id() < �ǹ�_���ó�);
                if ( city_a and !city_b) return true;
                if (!city_a and  city_b) return false;
                int troops_a = pk::get_troops(a);
                int troops_b = pk::get_troops(b);
                if (troops_a > 3000 and troops_b > 3000) return (int(a.hp) < int(b.hp));
                return (troops_a < troops_b);
            });
            
            pk::building@ target_base = list_target_base[0];
            pk::person@ leader = pk::get_person(unit.leader);
            
            if (pk::is_alive(target_base) and pk::is_alive(leader))
            {
                string leader_name = pk::decode(pk::get_name(leader));
                string target_name = pk::decode(pk::get_name(target_base));
                int target_id  = target_base.get_id();
                int unit_id    = unit.get_id();
                int max_atk_range   = get_atk_range(unit).second;
                pk::array<int> num_unit_around = count_unit_around(unit, target_base);  //[0]����, [1]����
                
                // �Ʊ� �������� �̵�
                if (arr_target_base_id[unit_id] != target_id)
                {   
                    // ���������δ� �̰ų�, ���������δ�鼭 �ֺ��� ���������� ���������δ밡 ���� ���
                    if (max_atk_range <= 1 or (max_atk_range > 1 and num_unit_around[0] < 1))
                    {
                        // ��ǥ �缳�� ���� ����
                        arr_target_base_id[unit_id] = target_id;
                        // �Ʊ� ���� �̵� �缳��
                        pk::set_order(unit, �δ��ӹ�_����, target_base.pos);  // �δ��ӹ�_�̵� �� �δ��ӹ�_���� ���� ���� ('20.9.13)
                        // ��ǥ���� �޽���
                        if (��ȭǥ�ü��� and pk::is_in_screen(unit.pos))
                        {
                            switch(pk::rand(3))
                            {
                                case 0: pk::say(pk::encode(pk::format("\x1b[2x{}\x1b[0x��(��) �����϶�", target_name)), leader); break;
                                case 1: pk::say(pk::encode(pk::format("�츮�� \x1b[2x{}\x1b[0x��(��) �����Ѵ�", target_name)), leader); break;
                                case 2: pk::say(pk::encode(pk::format("\x1b[2x{}\x1b[0x(��)�� ���϶�", target_name)), leader); break;
                            }
                        }
                        
                        if (����׸��)
                            pk::info( pk::format("{} �ӹ���ǥ:{}", leader_name, target_name) );
                        
                        return true;
                    }
                }
            }
            return false;
        }
        
        // �δ��ֺ��� �ڼ��� ����/���� ���� �δ�� Ȯ��
        pk::array<int> count_unit_around(pk::unit@ unit, pk::building@ target_base)
        {
            pk::array<int> count_unit(2, 0);    // [0]����, [1]����
            pk::array<pk::point> arr = pk::get_movable_pos(unit, target_base.pos, (unit.attr.stat[�δ�ɷ�_�̵�] / 2));
            for (int i = 0; i < int(arr.length); i++)
            {
                pk::unit@ unit_t = pk::get_unit(arr[i]);
                if (pk::is_alive(unit_t) and unit_t.type == �δ�����_����)
                    if (unit.get_id() != unit_t.get_id() and unit.get_force_id() == unit_t.get_force_id())
                    {
                        if (get_atk_range(unit_t).second > 1)
                             count_unit[1]++;
                        else count_unit[0]++;
                    }
            }
            return count_unit;
        }
        
        // ��� ���� �Ǵ� ����
        bool is_weak_base(pk::building@ base)
        {
            if (!pk::is_alive(base)) return false;
            int base_id = base.get_id();
            if (base_id < 0 or base_id >= �ǹ�_������) return false;
            bool is_city = (�ǹ�_���ý��� <= base_id and base_id < �ǹ�_���ó�);
            int base_troops = pk::get_troops(base);
            int base_hp     = int(base.hp);
            int base_max_hp = int(pk::get_max_hp(base));

            if (base_troops < 500 or base_hp < 700) return true;
            else if (!is_city and (base_troops <= 1000 and base_hp <= pk::max(1500, int(base_max_hp * 0.75f)))) return true;
            else if (!is_city and (base_troops <= 2000 and base_hp <= pk::max(1000, int(base_max_hp * 0.50f)))) return true;
            else if ( is_city and (base_troops <= 3000 and base_hp <= pk::max(2000, int(base_max_hp * 0.50f)))) return true;
            else if ( is_city and (base_troops <= 5000 and base_hp <= pk::max(1500, int(base_max_hp * 0.25f)))) return true;
            
            return false;
        }
        
        //---------------------------------------------------------------------------
        // ----- ���� �� ���� �δ� ���� �� �ʰ�����/���� �δ� ��ȯ ����
        // �����δ� : ���� ��ǥ������ ���� ���� Ȯ�� : true=���� �Ǵ� ���� / false=��� �Ǵ�
        bool retreat_exceed_unit(pk::unit@ unit)
        {
            if (!pk::is_alive(unit)) return false;
            int unit_id   = unit.get_id();
            int leader_id = unit.leader;
            pk::person@ leader = pk::get_person(leader_id);
            string unit_name = pk::decode(pk::get_name(leader));
            
            int candidate_count = list_candidate_unit.count;
            if (candidate_count > 0) return false;
            
            pk::building@ service_base = pk::get_building(pk::get_service(unit));
            pk::building@ pos_area_base = pk::get_building(pk::get_building_id(unit.pos));
            if (!pk::is_alive(service_base)) return false;
            if (!pk::is_alive(pos_area_base)) return false;
            int service_id = service_base.get_id();
            int pos_area_id  = pos_area_base.get_id();
            
            if (!pk::enemies_around(pos_area_base))
            {
                // ���� ���� �� �δ�+���� ���� �ѵ� ����
                bool is_service_base = (service_id == pos_area_id);
                bool is_force_base   = (unit.get_force_id() == pos_area_base.get_force_id());
                bool is_exceed_unit  = (is_force_base and !is_service_base and check_exceed_unit(pos_area_base, unit, true));
                
                // ���� �ѵ� �ʰ��ϴ� ��� �Ҽ� �������� ��
                if (is_exceed_unit and (arr_target_base_id[unit_id] == pos_area_id and arr_target_unit_id[unit_id] != -1))
                {
                    // ��ǥ �缳�� ���� ����
                    arr_target_base_id[unit_id] = service_id;
                    // �Ҽ� �������� ���ư�
                    pk::set_order(unit, �δ��ӹ�_����, service_base.pos);
                    // ��ǥ���� �޽���
                    if (��ȭǥ�ü��� and pk::is_in_screen(unit.pos))
                    {
                        string old_order = pk::format("\x1b[2x{}\x1b[0x�� ���� �ʾƵ� �ȴ�\n", pk::decode(pk::get_name(pos_area_base)));
                        string target_name = pk::decode(pk::get_name(service_base));
                        switch(pk::rand(3))
                        {
                            case 0: pk::say(pk::encode(pk::format("{}\x1b[2x{}\x1b[0x��(��) ���϶�", old_order, target_name)), leader); break;
                            case 1: pk::say(pk::encode(pk::format("{}�츮�� \x1b[2x{}\x1b[0x(��)�� ���ư���", old_order, target_name)), leader); break;
                            case 2: pk::say(pk::encode(pk::format("{}\x1b[2x{}\x1b[0x(��)�� ���Ѵ�", old_order, target_name)), leader); break;
                        }
                    }
                    if (����׸��) pk::info(pk::format("�δ��ȯ ��ǥ����: {}", pk::decode(pk::get_name(service_base))) );
                }
            }
            
            // �δ��ӹ� ���� ��ȸ
            bool is_order_retreat = (unit.order == �δ��ӹ�_�� or (unit.order == �δ��ӹ�_���� and candidate_count == 0));
                
            // �δ��ӹ� ��ǥ���� ���� �ѵ� �ʰ��ϴ� ��� �α��� ���º��� �������� ��
            if (is_order_retreat or unit.target_type == �δ��ӹ����_����)
            {
                pk::building@ base_t = (is_order_retreat)? service_base : pk::get_building(unit.target);
                if (pk::is_alive(base_t) and base_t.get_id() < �ǹ�_������)
                {
                    // �������� ��� ������ �δ����� �� ���� �ѵ� Ȯ��
                    string base_t_name     = pk::decode(pk::get_name(base_t));
                    int target_id          = base_t.get_id();
                    bool is_in_movable_pos = check_base_in_movable_range(unit, base_t); // �δ� �̵��Ÿ� ���� ��ǥ������ �����ϴ� ���
                    bool is_in_city_area   = (pk::get_city_id(base_t.pos) == pk::get_city_id(unit.pos));  // �δ���ġ�� ��ǥ������ ���� ���ÿ����� ���
                    bool is_force_base     = (unit.get_force_id() == base_t.get_force_id());
                    bool is_exceed_unit    = (is_force_base and (is_in_city_area or is_in_movable_pos) and check_exceed_unit(base_t, unit, true));
                    if (����׸��) pk::info(pk::format("---{}�� {}:{}", unit_name, get_order_info(unit.order), base_t_name));
                    
                    // ���� �ѵ� �ʰ��ϴ� ��� ���� ���� �α� �������� ��
                    if (is_exceed_unit)
                    { 
                        // ���� �˻� �Լ�
                        int dst_id = get_neighbor_base_retreat(base_t, unit);
                        pk::building@ dst_base = pk::get_building(dst_id);
                        if (!pk::is_alive(dst_base)) return false;
                        
                         // ��ǥ �缳�� ���� ����
                        arr_target_base_id[unit_id] = dst_id;
                        // �δ��ӹ� �缳�� : ���ο� ��ǥ �������� �̵�
                        pk::set_order(unit, �δ��ӹ�_����, dst_base.pos);
                        // ��ǥ���� �޽���
                        if (��ȭǥ�ü��� and pk::is_in_screen(unit.pos))
                        {
                            string old_order = pk::format("\x1b[2x{}\x1b[0x�� �����ϱ�\n", pk::decode(pk::get_name(base_t)));
                            string target_name = pk::decode(pk::get_name(dst_base));
                            switch(pk::rand(3))
                            {
                                case 0: pk::say(pk::encode(pk::format("{}\x1b[2x{}\x1b[0x(��)�� �����϶�", old_order, target_name)), leader); break;
                                case 1: pk::say(pk::encode(pk::format("{}�츮�� \x1b[2x{}\x1b[0x(��)�� ����", old_order, target_name)), leader); break;
                                case 2: pk::say(pk::encode(pk::format("{}\x1b[2x{}\x1b[0x(��)�� ���θ� �����Ѵ�", old_order, target_name)), leader); break;
                            }
                        }
                        if (����׸��) pk::info(pk::format("�δ��ȯ ��ǥ����: {}", pk::decode(pk::get_name(dst_base))) );
                    }
                }
            }
            
            return false;            
        }
        
        // ����� ���� ���� ������ ����
        pk::building@ src_t;
        int get_neighbor_base_retreat(pk::building@ src_base, pk::unit@ unit)
        {
            if (!pk::is_alive(unit) or !pk::is_alive(src_base)) return -1;
            int dst_id = -1;
            int best_dst = -1;
			int best_distance = 0;
            int src_id = src_base.get_id();
            pk::list<pk::building@> dst_list; 
            @src_t = @src_base;
            pk::force@ force = pk::get_force(src_base.get_force_id());
            
			// ö�� ���� �˻�
            if (pk::get_city_list(force).count <= 1 and src_id < �ǹ�_���ó�)
                return -1;      // ������ ������ ��� ö�� �Ұ�
            else
            {
                for (int i = 0; i < �ǹ�_������; i++)
                {
                    pk::building@ dst = pk::get_building(i);
                    int dst_id = dst.get_id();
                    bool is_exceed_unit = check_exceed_unit(dst, unit, true);
                    if (!is_exceed_unit and src_id != dst_id and src_base.get_force_id() == dst.get_force_id())
                    {
                        best_dst = dst_id;
                        dst_list.add(dst);  // ö������ ��������Ʈ �߰�
                    }
                }
            }
            
            // �������� ��������Ʈ ���� (���� �Ÿ� ��������, ��ǥ�Ÿ� ��������)
            if (dst_list.count == 0)
                best_dst = -1;
            else
            {
                dst_list.sort(function(a, b)
                {
                    int build_dist_a = pk::get_building_distance(a.get_id(), main.src_t.get_id(), a.get_force_id());
                    int build_dist_b = pk::get_building_distance(b.get_id(), main.src_t.get_id(), b.get_force_id());
                    int pos_dist_a = pk::get_distance(a.pos, main.src_t.pos);
                    int pos_dist_b = pk::get_distance(b.pos, main.src_t.pos);
                    if (build_dist_a != build_dist_b) return (build_dist_a < build_dist_b);
                    return (pos_dist_a < pos_dist_b);
                });
                best_dst = dst_list[0].get_id();
            }
			return best_dst;
        }
        
        
        // ����+�δ� ����/���� �ѵ� �ʰ� ���� Ȯ��
        bool check_exceed_unit(pk::building@ base, pk::unit@ unit, bool isforce=true)
        {
            if (!pk::is_alive(base) or !pk::is_alive(unit)) return false;
            if (base.get_id() >= �ǹ�_������) return false;
            if ( isforce and base.get_force_id() != unit.get_force_id()) return false;
            if (!isforce and !pk::is_enemy(base, unit)) return false;
            
            float exceed_limit  = pk::max(0.0f, pk::min(1.0f, ���Ժδ�_�ʰ��ѵ�����));
            float weapon_ignore = pk::max(0.0f, pk::min(1.0f, ���Ժδ�_�ʰ�����_�������±���));
            bool is_ignore = (float(pk::get_troops(base))/float(pk::get_max_troops(base)) <= weapon_ignore);
            
            bool is_exceed_troops = false;
            bool is_exceed_weapon = false;
            if (unit.type == �δ�����_����)
            {
                if (���Ժδ�_�ʰ����±�ȯ)
                {
                    int base_limit = int(float(pk::get_max_troops(base)) * exceed_limit);
                    int exceed_troops = pk::max(0, ((pk::get_troops(base) + unit.troops) - base_limit));
                    is_exceed_troops = (exceed_troops > 0);
                }
                if (���Ժδ�_�ʰ������ȯ and !is_ignore)
                {
                    int gnd_wpn_id = pk::get_ground_weapon_id(unit);
                    int sea_wpn_id = pk::get_sea_weapon_id(unit);
                    int unit_gnd_wpn_amt = (is_siege_weapon(gnd_wpn_id))? 1 : unit.troops;
                    int unit_sea_wpn_amt = (sea_wpn_id == ����_�ְ�)? 0 : 1;
                    int base_gnd_wpn_amt = pk::get_weapon_amount(base, gnd_wpn_id);
                    int base_sea_wpn_amt = pk::get_weapon_amount(base, sea_wpn_id);
                    int exceed_gnd_wpn_amt = pk::max(0, (base_gnd_wpn_amt + unit_gnd_wpn_amt) - int(float(pk::get_max_weapon_amount(base, gnd_wpn_id))*exceed_limit) );
                    int exceed_sea_wpn_amt = pk::max(0, (base_sea_wpn_amt + unit_sea_wpn_amt) - int(float(pk::get_max_weapon_amount(base, sea_wpn_id))*exceed_limit) );
                    is_exceed_weapon = (exceed_gnd_wpn_amt > 0 or exceed_sea_wpn_amt > 0);
                }
            }
            else if (unit.type == �δ�����_����)
            {
                if (���Ժδ�_�ʰ����±�ȯ)
                {
                    int exceed_troops = pk::max(0, ((pk::get_troops(base) + unit.troops) - pk::get_max_troops(base)) );
                    is_exceed_troops = (exceed_troops >= 2000);  // �δ뺴�� 2000 �̻� ���� �� 
                }
                if (���Ժδ�_�ʰ������ȯ and !is_ignore)
                {
                    pk::array<int> arr_exceed_wpn_amt(����_��, 0);
                    for (int i = 0; i < ����_��; i++)
                    {
                        if (!is_exceed_weapon and !is_ignore)
                        {
                            int unit_wpn_amt = unit.stock[i].amount;
                            int base_wpn_amt = pk::get_weapon_amount(base, i);
                            arr_exceed_wpn_amt[i] = pk::max(0, (unit_wpn_amt + unit_wpn_amt) - int(float(pk::get_max_weapon_amount(base, i))*exceed_limit) );
                            if (arr_exceed_wpn_amt[i] >= ((is_siege_weapon(i) or is_ship_weapon(i))? 1 : 2000))
                                is_exceed_weapon = true;  // ����/�Լ����� 1�̻� �Ǵ� �������� 2000 �̻� ���� ��
                        }
                    }
                }
            }
            
            if (����׸��)
            {
                string unit_name = pk::decode(pk::get_name(pk::get_person(unit.leader)));
                string base_name = pk::decode(pk::get_name(base));
                if (is_exceed_troops or is_exceed_weapon)
                    pk::info(pk::format("---{}/{}�� ���Խ� ����ʰ�", base_name, unit_name));
            }
            bool is_exceed_unit = (is_exceed_troops or is_exceed_weapon);
            return is_exceed_unit;
        }
        
        // �δ��� �̵����� ���� ��ǥ������ ��ġ�ϴ��� Ȯ��
        bool check_base_in_movable_range(pk::unit@ unit, pk::building@ base)
        {
            if (!pk::is_alive(unit) or !pk::is_alive(base)) return false;
            
            pk::array<pk::point> arr_range = pk::get_movable_pos(unit);;
            for (int i = 0; i < int(arr_range.length); i++)
            {
                if (base.pos == arr_range[i]) return true;
            }
            return false;
        }
        
        bool is_siege_weapon(int weapon_id) { return (����_���� <= weapon_id and weapon_id <= ����_���); }
        bool is_ship_weapon(int weapon_id) { return (����_���� == weapon_id or weapon_id == ����_����); }
        
        
        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------
        
        // �������� �δ����� Ȯ��
        bool check_valid_unit(pk::unit@ unit_t)
        {
            if (!pk::is_alive(unit_t))      return false;
			if (unit_t.type != �δ�����_����)	return false;	// ���� �δ� ����
            
            int force_id = unit_t.get_force_id();
            pk::force@ force = pk::get_force(force_id);
			if (!pk::is_alive(force) or !pk::is_normal_force(force_id)) return false;  // �Ҽ� ���� Ȯ��  
			if (!pk::is_alive(pk::get_building(pk::get_service(unit_t))))	return false;	// �ҼӰ��� Ȯ�κҰ� ����
            
            int leader_id = unit_t.leader;
            if ((leader_id >= 670 and leader_id < 800) or (leader_id >= 1000 and leader_id < 1100)) return false;
            
            return true;
        }
        
        
        // ���� Ȯ�� �Լ�
        bool check_force_option(pk::unit@ unit_t)
        {
            // �÷��̾� ���� ������ (������ �ӹ����� �����ϸ鼭 ��ǥ�缳���ϹǷ� ������ ����)
            if (!pk::is_alive(unit_t)) return false;
            if (pk::is_player_controlled(unit_t)) 
            {
                if (�δ��ǥ����_������������) return true;
                else return false;   
            }
            
            // 0: ��ǻ��AI �� �÷��̾�_���ӱ��� ���,  1: �÷��̾�_���ӱ��ܸ�, 2: ��ǻ��AI��,  3: ��� ������
            if      (�δ��ǥ����_�������� == 0 and !pk::is_player_controlled(unit_t)) return true;
            else if (�δ��ǥ����_�������� == 1 and unit_t.is_player() and !pk::is_player_controlled(unit_t)) return true;
            else if (�δ��ǥ����_�������� == 2 and !unit_t.is_player()) return true;
            else if (�δ��ǥ����_�������� == 3) return false;
            
            return false;
        }
        
        // �Ǽ����� ���� �Ǵ� �Լ�
        bool check_construction_unit(pk::unit@ unit)
        {
            if (!pk::is_alive(unit)) return false;
            if (pk::is_player_controlled(unit)) return false; // ���������� ������ ����
            if (unit.has_skill(Ư��_�༺) and unit.gold >= �Ǽ�����_���ؼ�����) return true;  // �༺ Ư��
            if (unit.type == �δ�����_���� and pk::get_member_count(unit) <= 1)   // 1�δ�
            {
                if (unit.gold >= �Ǽ�����_���ؼ����� and unit.troops <= �Ǽ�����_���غ���) 
                    return true; 
            }
            if (�Ǽ�����_�ӹ���� and unit.order == �δ��ӹ�_��ġ and unit.target_type == �δ��ӹ����_��ǥ)
            {
                int facility_id = unit.target;
                pk::facility@ facility = pk::get_facility(facility_id);
                if (pk::is_alive(facility) and int(unit.gold) >= int(facility.gold_cost))
                {
                    if (�ü�_�� <= facility_id and facility_id <= �ü�_��������)
                        return true;
                }
            }
            
            return false;
        }
        
        // �跫�δ� ���� �Ǵ� �Լ�
        int func_strategy_unit_type(pk::unit@ unit)
        {
            if (!pk::is_alive(unit)) return -1;
            if (pk::is_player_controlled(unit)) return -1; // ���������� ������ ����
            
            // ������ ��� Ư��
            if (unit.has_skill(Ư��_�Ż�) or unit.has_skill(Ư��_���) or unit.has_skill(Ư��_����) 
                or unit.has_skill(Ư��_�͸�) or unit.has_skill(Ư��_��ȯ))
                return 3;
            
            // ���� Ư�� Ȯ��
            for (int i = 0; i < 3; i++)
            {
                pk::person@ member = pk::get_person(unit.member[i]);
                if (pk::is_alive(member))
                {
                    int skill_id = member.skill;
                    pk::skill@ skill = pk::get_skill(skill_id);
                    
                    // ��/�� �迭 ����Ư���� ��� �跫�δ� �ƴ� ������ ���� (���°���� ����)
                    if (skill_id >= Ư��_â�� and skill_id <= Ư��_����) return 0;   
                    
                    // �跫Ư�� ���
                    if (�跫�δ�_Ư���� and pk::is_alive(skill))
                    {
                        if (skill.type == Ư������_�跫) return 2;    
                    }
                }
            }
            
            // ���� �� ���� ���� �Ǵ� (�Ϲݺδ�)
            float stat_ratio = float(2.0f * unit.attr.stat[�δ�ɷ�_����] / (unit.attr.stat[�δ�ɷ�_����] + unit.attr.stat[�δ�ɷ�_���]));
            if (stat_ratio >= �跫�δ�_���°����) return 1;
            
            return 0;
        }
        
        
        //---------------------------------------------------------------------------
        
        // �δ� ���� �ּ�,�ִ� ��Ÿ� �Լ� -- �ű� �߰�, ��밡���� ������ �������ǿ� ���� ��� ('20.9.13)
        pk::int_int get_atk_range(pk::unit@ unit)
        {
            pk::int_int atk_range = pk::int_int(1, 1);
            if (!pk::is_alive(unit) or unit.weapon == ����_��) return atk_range;
        
            int weapon_id = unit.weapon;
            pk::equipment@ epq = pk::get_equipment(unit.weapon);
            string eqp_name = pk::decode(pk::get_name(epq));
            string info = pk::format("{}", eqp_name);
            
            int min_range = 5;
            int max_range = 1;
            
            for (int j = 0; j < ����_��; j++)
            {
                if (epq.tactics[j])
                {
                    pk::tactics@ tts = pk::get_tactics(j);
                    string tts_name = pk::decode(pk::get_name(tts));
                    info += pk::format(" {}({},{})", tts_name, tts.min_range, tts.max_range);
                    min_range = pk::min(min_range, tts.min_range);
                    max_range = pk::max(max_range, tts.max_range);
                }
            }
            if (weapon_id == ����_�� and unit.has_tech(�ⱳ_����)) max_range += 1;
            if (weapon_id == ����_���� and (unit.has_tech(�ⱳ_���) or unit.has_skill(Ư��_�鸶))) max_range = pk::max(2, max_range);
            if ((weapon_id == ����_���� or weapon_id == ����_����) and unit.has_skill(Ư��_����)) max_range += 1;
            
            atk_range = pk::int_int(min_range, max_range);
            info += pk::format(": �ּ�({}),�ִ�({})", min_range, max_range);
            
            //if (����׸��) pk::info(info);
            
            return atk_range;
        }
        
        // �δ� ���� �ִ� ��Ÿ� �Լ�
        int get_max_atk_range(pk::unit@ unit)
        {
            if (!pk::is_alive(unit)) return 1;
            
            int weapon_id = unit.weapon;
            int max_range = 0;
            
            if      (weapon_id == ����_��)     max_range = (unit.has_tech(�ⱳ_����))? 3 : 2;
            else if (weapon_id == ����_����)   max_range = (unit.has_tech(�ⱳ_���) or unit.has_skill(Ư��_�鸶))? 2 : 1;
            else if (weapon_id == ����_����)   max_range = ((unit.has_skill(Ư��_����))? 1 : 0) + (pk::get_tactics(����_����ȭ��).max_range);
            else if (weapon_id == ����_����)   max_range = ((unit.has_skill(Ư��_����))? 1 : 0) + (pk::get_tactics(����_��������).max_range);
            else if (weapon_id >= ����_�ְ�)   max_range = 2;
            else                             max_range = 1;     // ��, â, ��, ��� ��
            
            return max_range;
        }
        
        // ��ǥ�δ� ����ġ ��� �Լ� (��, ����, ����, �ź� ���)
        float get_attr_weight(pk::unit@ unit)
        {
            if (!pk::is_alive(unit)) return 0.0f;
            int weapon_id  = unit.weapon;
            int heishu_id  = pk::equipment_id_to_heishu(weapon_id);
            int tekisei    = pk::min(����_S, pk::get_tekisei(unit));
            int unit_atk   = unit.attr.stat[�δ�ɷ�_����];
            int unit_def   = unit.attr.stat[�δ�ɷ�_���];
            int unit_skill = func_strategy_unit_type(unit);
            
            if (heishu_id < 0 or heishu_id >= 6) return 0.0f;
            
            float relative_stat = 0.0f;
            float weight  = main.heishu_weight[heishu_id] ;
            
            // ��, ���� ����ġ ����
            relative_stat = pk::max(0.f, weight) + float(����_�� - tekisei)/float(����_��) ;
            
            // ���ݷ� ����ġ ����
            if (�δ��ǥ����_���ݰ���ġ����) relative_stat += float(unit_atk)/128.0f;
            // �濩�� ����ġ ����
            if (�δ��ǥ����_����ġ����) relative_stat += float(unit_def)/128.0f;
                        
            // ����/���� �ź� ����ġ ����
            if (�δ��ǥ����_�źа���ġ����)
            {
                if      (unit.leader == pk::get_kunshu_id(unit)) relative_stat += 0.50f;
                else if (unit.leader == pk::get_totoku_id(unit)) relative_stat += 0.25f;
            }
            
            // ���� ����ġ ���� ('19.3.1)
            if (�δ��ǥ����_���°���ġ����)
            {
                if (int(unit.troops) <= 5000)
                    relative_stat += float(5000 - unit.troops)/10000.f;
            }
            
            // �跫��� Ư�� ���� ����ġ ���� ('20.8.30)
            if (�δ��ǥ����_�跫����ġ���� and (unit_skill > 1)) relative_stat += 0.4f * float(unit_skill);
            
            return relative_stat;
        }
        
        // ������ �����Լ�
        void set_heishu_weight(int weapon_id)
        {
            int heishu_id = pk::equipment_id_to_heishu(weapon_id);
            
            if (heishu_id == ����_â��)
            {
                heishu_weight[����_â��] = heishu_normal;
                heishu_weight[����_�غ�] = heishu_weak  ;
                heishu_weight[����_�뺴] = heishu_normal;
                heishu_weight[����_�⺴] = heishu_strong;
                heishu_weight[����_����] = heishu_strong;
                heishu_weight[����_����] = heishu_null  ;
            }
            else if (heishu_id == ����_�غ�)
            {        
                heishu_weight[����_â��] = heishu_strong;
                heishu_weight[����_�غ�] = heishu_normal;
                heishu_weight[����_�뺴] = heishu_normal;
                heishu_weight[����_�⺴] = heishu_weak  ;
                heishu_weight[����_����] = heishu_strong;
                heishu_weight[����_����] = heishu_null  ;
            }
            else if (heishu_id == ����_�뺴)
            {  
                heishu_weight[����_â��] = heishu_normal;
                heishu_weight[����_�غ�] = heishu_weak  ;
                heishu_weight[����_�뺴] = heishu_normal;
                heishu_weight[����_�⺴] = heishu_strong;
                heishu_weight[����_����] = heishu_strong;
                heishu_weight[����_����] = heishu_normal;
            }
            else if (heishu_id == ����_�⺴)
            { 
                heishu_weight[����_â��] = heishu_weak  ;
                heishu_weight[����_�غ�] = heishu_strong;
                heishu_weight[����_�뺴] = heishu_strong;
                heishu_weight[����_�⺴] = heishu_normal;
                heishu_weight[����_����] = heishu_strong;
                heishu_weight[����_����] = heishu_null  ;
            }
            else if (heishu_id == ����_����)
            { 
                heishu_weight[����_â��] = heishu_normal;
                heishu_weight[����_�غ�] = heishu_normal;
                heishu_weight[����_�뺴] = heishu_normal;
                heishu_weight[����_�⺴] = heishu_normal;
                heishu_weight[����_����] = heishu_normal;
                heishu_weight[����_����] = heishu_null  ;
            }
            else if (heishu_id == ����_����)
            { 
                heishu_weight[����_â��] = heishu_normal;
                heishu_weight[����_�غ�] = heishu_normal;
                heishu_weight[����_�뺴] = heishu_normal;
                heishu_weight[����_�⺴] = heishu_normal;
                heishu_weight[����_����] = heishu_weak  ;
                heishu_weight[����_����] = heishu_strong;
            }
            else
            { 
                heishu_weight[����_â��] = heishu_weak  ;
                heishu_weight[����_�غ�] = heishu_weak  ;
                heishu_weight[����_�뺴] = heishu_weak  ;
                heishu_weight[����_�⺴] = heishu_weak  ;
                heishu_weight[����_����] = heishu_normal;
                heishu_weight[����_����] = heishu_null  ;
            }
        }
        
        
        //---------------------------------------------------------------------------
        //---------------------------------------------------------------------------
        
        int get_weapon_type (int weapon_id)
        {
            int weapon_type = -1;
            if      (weapon_id == ����_��)                          weapon_type = 0;
            else if (weapon_id >= ����_â and weapon_id <= ����_����)  weapon_type = 1;
            else if (weapon_id == ����_����)                         weapon_type = 2;
            else if (weapon_id >= ����_���� and weapon_id <= ����_���) weapon_type = 3;
            else if (weapon_id == ����_�ְ�)                         weapon_type = 4;
            else if (weapon_id >  ����_�ְ�)                         weapon_type = 5;
            
            return weapon_type;
        }
        
        string get_weapon_name(int weapon_id)
        {
            string weapon_name;
            switch(weapon_id)
            {
                case ����_��  : weapon_name = "�˺�"; break;
                case ����_â  : weapon_name = "â��"; break;
                case ����_��  : weapon_name = "�غ�"; break;
                case ����_��  : weapon_name = "�뺴"; break;
                case ����_���� : weapon_name = "�⺴"; break;
                case ����_���� : weapon_name = "����"; break;
                case ����_���� : weapon_name = "����"; break;
                case ����_���� : weapon_name = "����"; break;
                case ����_��� : weapon_name = "���"; break;
                case ����_�ְ� : weapon_name = "����"; break;
                case ����_���� : weapon_name = "����"; break;
                case ����_���� : weapon_name = "����"; break;
                default     : weapon_name = "??"; break;
            }
            return weapon_name;
        }
        
        
        string get_tekisei_name(int tekisei)
        {
            string tekisei_name;
            switch(tekisei)
            {
                case ����_C  : tekisei_name = "C"; break;
                case ����_B  : tekisei_name = "B"; break;
                case ����_A  : tekisei_name = "A"; break;
                case ����_S  : tekisei_name = "S"; break;
                default    : tekisei_name = "?"; break;
            }
            return tekisei_name;
        }
        
        
        string get_heishu_color(int weapon_id)
        {
            string text_color;
            float weight  = main.heishu_weight[pk::equipment_id_to_heishu(weapon_id)];
            
            if      (weapon_id == ����_��)      text_color = "\x1b[2x"; // ���
            else if (weight == heishu_null  ) text_color = "\x1b[29x"; // ����
            else if (weight == heishu_weak  ) text_color = "\x1b[16x"; // ��Ȳ
            else if (weight == heishu_normal) text_color = "\x1b[17x"; // ���
            else if (weight == heishu_strong) text_color = "\x1b[2x"; // ���
            else                              text_color = "\x1b[0x"; // ���
                
            return text_color;
        }
        
        //---------------------------------------------------------------------------
        
        void clear_target_info()
        {
            for (int i = 0; i < �δ�_��; i++)
            {
                arr_target_unit_id[i] = -1;
                arr_target_base_id[i] = -1;
            }
        }
        
        //---------------------------------------------------------------------------------------
        
        string get_order_info(int order)
        {
            string name;
            switch(order)
            {
                case 0  : name = "�δ��ӹ�_���"; break;
                case 1  : name = "�δ��ӹ�_�̵�"; break;
                case 2  : name = "�δ��ӹ�_��ġ"; break;
                case 3  : name = "�δ��ӹ�_����"; break;
                case 4  : name = "�δ��ӹ�_��"; break;
                case 5  : name = "�δ��ӹ�_����"; break;
                case 6  : name = "�δ��ӹ�_ȣ��"; break;
                case 7  : name = "�δ��ӹ�_����"; break;
                case 8  : name = "�δ��ӹ�_����"; break;
                case 9  : name = "�δ��ӹ�_����"; break;
                case 10 : name = "�δ��ӹ�_����"; break;
                case 11 : name = "�δ��ӹ�_����"; break;
                case 12 : name = "�δ��ӹ�_����"; break;
                case 13 : name = "�δ��ӹ�_��" ; break;
                default : name = "�δ��ӹ�_����"; break;
            }
            
            return name;
        }
        

        bool ����׸�� = false;
	}

    
	Main main;
}

