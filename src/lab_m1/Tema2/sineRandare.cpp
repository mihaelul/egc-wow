#include "sineRandare.h"
#include <iostream>
namespace sineRandare
{

    glm::vec3 CalculateRailEnd(const RailSegment& r) {
        return r.start + DirToVector(r.dir) * r.length;
    }

    void BuildRailMap(std::vector<RailSegment>& railMap)
    {
        railMap.clear();

        const float L = 1.5f;
        const float overlap = 0.3f;

        glm::vec3 cursor = { -4.5f, 0.05f, -4.5f };
        glm::vec3 cursorInter;

        // =====================
        // segment 1
        // =====================
        for (int i = 0; i < 6; i++)
        {
            if(i==3)
                railMap.push_back({ RailType::Normal, RailDir::Forward, L, cursor, StationType::Sphere });
			else
            railMap.push_back({ RailType::Normal, RailDir::Forward, L, cursor });
            cursor.z += L;
        }


        cursor.x -= overlap;

        // =====================
        // Segment 2
        // =====================
        for (int i = 0; i < 2; i++)
        {
            railMap.push_back({ RailType::Normal, RailDir::Right, L, cursor });
            cursor.x += L;
        }


        railMap.push_back({ RailType::Bridge, RailDir::Right, L, cursor });
        cursor.x += L;


        cursor.x += overlap;


        for (int i = 0; i < 3; i++)
        {
            railMap.push_back({ RailType::Normal, RailDir::Right, L, cursor });
            cursor.x += L;
        }


        cursor.z -= overlap;

        // =====================
        // Segment 3
        // =====================
        for (int i = 0; i < 4; i++)
        {
            railMap.push_back({ RailType::Normal, RailDir::Forward, L, cursor });
            cursor.z += L;
        }

        railMap.push_back({ RailType::Tunnel, RailDir::Forward, L, cursor });

        cursor.z += overlap + 1.5f;

        for (int i = 0; i < 2; i++)
        {
            if (i == 0)
                railMap.push_back({ RailType::Normal, RailDir::Forward, L, cursor });
            else
                railMap.push_back({ RailType::Normal, RailDir::Forward, L, cursor });
            cursor.z += L;
        }
        // =====================
        // Segment 4
        // =====================

        for (int i = 0; i < 3; i++)
        {
            railMap.push_back({ RailType::Normal, RailDir::Right, L, cursor });
            cursor.x += L;
        }

        for (int i = 0; i < 2; i++)
        {
            railMap.push_back({ RailType::Tunnel, RailDir::Right, L, cursor });
            cursor.x += L;
        }

        cursor.x += overlap;

        for (int i = 0; i < 7; i++)
        {
            if (i == 2)
            {
                railMap.push_back({ RailType::Normal, RailDir::Right, L, cursor });
            }
            else
                railMap.push_back({ RailType::Normal, RailDir::Right, L, cursor });
            cursor.x += L;
        }

        // =====================
      // Segment 5
      // =====================

        cursor.z += overlap;

        for (int i = 0; i < 2; i++)
        {
            railMap.push_back({ RailType::Normal, RailDir::Backward, L, cursor });
            cursor.z -= L;
        }

        cursor.x += overlap;

        // =====================
        // Segment 6
        // =====================


        for (int i = 0; i < 2; i++)
        {
            if (i == 0)
            {
                railMap.push_back({ RailType::Normal, RailDir::Left, L, cursor, StationType::Cube });
            }
            else
                railMap.push_back({ RailType::Normal, RailDir::Left, L, cursor });
            cursor.x -= L;
        }

        cursor.z += overlap;

        // =====================
        // Segment 7
        // =====================
        for (int i = 0; i < 4; i++)
        {
            railMap.push_back({ RailType::Normal, RailDir::Backward, L, cursor });

            glm::vec3 endBack = cursor + glm::vec3(0, 0, -L);
            if (i == 1)
            {

                railMap.push_back({ RailType::Normal, RailDir::Left, L, endBack });

                glm::vec3 secondLeft =
                    endBack + glm::vec3(-L, 0, 0);

                railMap.push_back({ RailType::Normal, RailDir::Left, L, secondLeft });

                secondLeft += glm::vec3(-L, 0, 0);

                railMap.push_back({ RailType::Normal, RailDir::Left, L, secondLeft });

                secondLeft += glm::vec3(-L, 0, 0);

                railMap.push_back({ RailType::Normal, RailDir::Left, L, secondLeft });
                secondLeft += glm::vec3(-L, 0, 0);

                railMap.push_back({ RailType::Normal, RailDir::Left, L, secondLeft });


            }
            cursor.z -= L;
        }

        // =====================
        // Segment 8
        // =====================
        cursor.x -= overlap;
        for (int i = 0; i < 3; i++)
        {
            if (i == 1)
            {
                railMap.push_back({ RailType::Normal, RailDir::Right, L, cursor, StationType::Pyramid });
            }

            railMap.push_back({ RailType::Normal, RailDir::Right, L, cursor });
            cursor.x += L;

        }


        for (int i = 0; i < 4; i++)
        {
            railMap.push_back({ RailType::Tunnel, RailDir::Right, L, cursor });
            cursor.x += L;
        }

        cursor.x += overlap;

        for (int i = 0; i < 2; i++)
        {
            railMap.push_back({ RailType::Normal, RailDir::Right, L, cursor });
            cursor.x += L;
        }

        // =====================
       // Segment 9
       // =====================

        cursor.z += overlap;
        for (int i = 0; i < 4; i++)
        {

            railMap.push_back({ RailType::Normal, RailDir::Backward, L, cursor });
            cursor.z -= L;
        }

        railMap.push_back({ RailType::Bridge, RailDir::Backward, L, cursor });
        cursor.z -= L;


        cursor.z -= overlap;
        for (int i = 0; i < 2; i++)
        {
            railMap.push_back({ RailType::Normal, RailDir::Backward, L, cursor });
            cursor.z -= L;
        }

        cursor.x += overlap;
        cursorInter.x = cursor.x;
        cursorInter.y = cursor.y;
        cursorInter.z = cursor.z;

        for (int i = 0; i < 22; i++)
        {
            railMap.push_back({ RailType::Normal, RailDir::Left, L, cursor });
            glm::vec3 currentStart = cursor;
            glm::vec3 endLeft = currentStart + glm::vec3(-L, 0, -0.3f);

            if (i == 20)
            {
                railMap.push_back({ RailType::Normal, RailDir::Forward, L, endLeft });


                glm::vec3 secondForward =
                    endLeft + glm::vec3(0, 0, L+0.2f);

                railMap.push_back({ RailType::Normal, RailDir::Forward, L, secondForward });

                secondForward += glm::vec3(0, 0, L);
                railMap.push_back({ RailType::Bridge, RailDir::Forward, L, secondForward });

                secondForward += glm::vec3(0, 0, L+0.3f);
                railMap.push_back({ RailType::Normal, RailDir::Forward, L, secondForward });

                secondForward += glm::vec3(-0.3f, 0, L);
                railMap.push_back({ RailType::Normal, RailDir::Right, L, secondForward });
                for (int i = 0; i < 6; i++)
                {

                    secondForward += glm::vec3(L, 0, 0);
                    railMap.push_back({ RailType::Normal, RailDir::Right, L, secondForward });
                }

                secondForward += glm::vec3(L, 0, 0.f);
                railMap.push_back({ RailType::Normal, RailDir::Forward, L, secondForward });

                for (int i = 0; i < 3; i++)
                {
                    if (i == 2)
                    {
                        glm::vec3 forthIntersection =
                            secondForward + glm::vec3(-L, 0, 0);
                        //stanga
                        railMap.push_back({ RailType::Normal, RailDir::Left, L, secondForward });
                        railMap.push_back({ RailType::Normal, RailDir::Left, L, forthIntersection });

                        forthIntersection += glm::vec3(-L, 0, 0);
                        railMap.push_back({ RailType::Normal, RailDir::Left, L, forthIntersection, StationType::Cube });

                        forthIntersection += glm::vec3(-L, 0, 0);
                        railMap.push_back({ RailType::Normal, RailDir::Left, L, forthIntersection });
                        forthIntersection += glm::vec3(-L, 0, 0);
                        railMap.push_back({ RailType::Normal, RailDir::Left, L, forthIntersection });



                        //dreapta
                        railMap.push_back({ RailType::Normal, RailDir::Right, L, secondForward, StationType::Sphere });
                    }
                    secondForward += glm::vec3(0, 0, L);
                    railMap.push_back({ RailType::Normal, RailDir::Forward, L, secondForward });
                }

            }
            cursor.x -= L;
        }

        cursor.x += overlap + 0.3f;

        railMap.push_back({RailType::Normal, RailDir::Left, L, cursor});
        cursor.x -= L;

        railMap.push_back({ RailType::Bridge, RailDir::Left, L, cursor });
        cursor.x -= L;


        cursor.x -= overlap;
        for (int i = 0; i < 2; i++)
        {
            railMap.push_back({ RailType::Normal, RailDir::Left, L, cursor });
            cursor.x -= L;
        }
        


    }



    void BuildRailGraph(std::vector<RailSegment>& railMap, float eps)
    {
        for (auto& r : railMap)
        {
            r.nextAtEnd.clear();
            r.forwardAtEnd.clear();
            r.nextAtStart.clear();
            r.forwardAtStart.clear();
            r.isIntersection = false;
        }

        for (int i = 0; i < (int)railMap.size(); i++)
        {
            glm::vec3 startI = railMap[i].start;
            glm::vec3 endI = CalculateRailEnd(railMap[i]);

            for (int j = 0; j < (int)railMap.size(); j++)
            {
                if (i == j) continue;

                glm::vec3 startJ = railMap[j].start;
                glm::vec3 endJ = CalculateRailEnd(railMap[j]);


                if (glm::distance(endI, startJ) <= eps) {
                    railMap[i].nextAtEnd.push_back(j);
                    railMap[i].forwardAtEnd.push_back(true);
                }

                else if (glm::distance(endI, endJ) <= eps) {
                    railMap[i].nextAtEnd.push_back(j);
                    railMap[i].forwardAtEnd.push_back(false);
                }


                if (glm::distance(startI, startJ) <= eps) {
                    railMap[i].nextAtStart.push_back(j);
                    railMap[i].forwardAtStart.push_back(true);
                }

                else if (glm::distance(startI, endJ) <= eps) {
                    railMap[i].nextAtStart.push_back(j);
                    railMap[i].forwardAtStart.push_back(false);
                }
            }
        }


        MarkIntersections(railMap);
    }
    glm::vec3 GetWorldHeading(const RailSegment& nextRail, bool entersForward) {
        glm::vec3 dirVec;
        switch (nextRail.dir) {
        case RailDir::Forward:  dirVec = glm::vec3(0, 0, 1);  break;
        case RailDir::Backward: dirVec = glm::vec3(0, 0, -1); break;
        case RailDir::Right:    dirVec = glm::vec3(1, 0, 0);  break;
        case RailDir::Left:     dirVec = glm::vec3(-1, 0, 0); break;
        }
        return entersForward ? dirVec : -dirVec; 
    }

    void MarkIntersections(std::vector<RailSegment>& railMap) {
        for (auto& r : railMap) {
         
            if (r.nextAtEnd.size() > 1) {
                std::vector<glm::vec3> uniqueDirections;
                for (size_t i = 0; i < r.nextAtEnd.size(); i++) {
                    glm::vec3 heading = GetWorldHeading(railMap[r.nextAtEnd[i]], r.forwardAtEnd[i]);

                    
                    bool exists = false;
                    for (const auto& d : uniqueDirections) {
                        if (glm::distance(d, heading) < 0.1f) { exists = true; break; }
                    }
                    if (!exists) uniqueDirections.push_back(heading);
                }
                r.isIntersection = (uniqueDirections.size() >= 2);
            }

            
            if (r.nextAtStart.size() > 1) {
                std::vector<glm::vec3> uniqueDirections;
                for (size_t i = 0; i < r.nextAtStart.size(); i++) {
                    glm::vec3 heading = GetWorldHeading(railMap[r.nextAtStart[i]], r.forwardAtStart[i]);
                    bool exists = false;
                    for (const auto& d : uniqueDirections) {
                        if (glm::distance(d, heading) < 0.1f) { exists = true; break; }
                    }
                    if (!exists) uniqueDirections.push_back(heading);
                }
                
                if (uniqueDirections.size() >= 2) r.isIntersection = true;
            }
        }
    }

}