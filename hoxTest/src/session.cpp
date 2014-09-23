//
// C++ Implementation: session
//
// Description: The main session handler
//
// Author: Huy Phan,,,, (C) 2009
//
// Created: 04/12/2009
//

#include <st.h>
#include <boost/tokenizer.hpp>
#include <sstream>
#include <map>
#include <memory>

#include "main.h"
#include "hoxCommon.h"
#include "hoxLog.h"
#include "hoxExcept.h"
#include "hoxDebug.h"
#include "TestPlayer.h"
#include "Manager.h"
#include "AIPlayer.h"

/******************************************************************
 * Implementation
 */

/**
 * Handle session for Test players.
 */
void
handle_session( const int thread_id )
{
    hoxLog(LOG_INFO, "%s: [#%d]: ENTER.", __FUNCTION__, thread_id);

    TestPlayer_SPtr player;
    Game*           game = NULL;  // The "assigned" game.

    for (;;)
    {
        if ( player )
        {
            Manager::instance()->onPlayerDisconnected( player, game );
            player.reset();
        }

        game = NULL;
        player = Manager::instance()->createTestPlayer( thread_id, game );
        if ( ! player )
        {
            hoxLog(LOG_DEBUG, "%s: [#%d] waiting for game...", __FUNCTION__, thread_id);
            st_sleep( 60 /* in seconds */ );
            continue;
        }

        try
        {
            player->run();
            st_sleep( 60 /* seconds */ ); // Wait before continue.
        }
        catch (std::runtime_error& ex)
        {
            hoxLog(LOG_WARN, "%s: [#%d] caught runtime exception [%s].",
                    __FUNCTION__, thread_id, ex.what());
        }
    }

    // Cleanup again just to be sure!
    if ( player )
    {
        Manager::instance()->onPlayerDisconnected( player, game );
    }

    hoxLog(LOG_INFO, "%s: [#%d]: END.", __FUNCTION__, thread_id);
}

//////////////////////////////////////////////////////////////////////////////

/**
 * AI thread.
 */
void*
AI_Player_thread( void* arg )
{
    std::auto_ptr<hoxAIConfig> pAIConfig( (hoxAIConfig*) arg );

    const std::string sPIDs = hoxUtil::joinElements<hoxStringVector>(pAIConfig->pids);
    hoxLog(LOG_INFO,  "%s: pids      = [%s]", __FUNCTION__, sPIDs.c_str() );
    hoxLog(LOG_DEBUG, "%s: password  = [%s]", __FUNCTION__, pAIConfig->password.c_str() );
    hoxLog(LOG_DEBUG, "%s: ai_engine = [%s]", __FUNCTION__, pAIConfig->engine.c_str() );
    hoxLog(LOG_DEBUG, "%s: ai_role   = [%s]", __FUNCTION__, pAIConfig->role.c_str() );
    hoxLog(LOG_DEBUG, "%s: ai_depth  = [%d]", __FUNCTION__, pAIConfig->depth );

    size_t      pindex = 9999;  /* The index of the chosen PID.
                                 * Note; Use large value to use 1st pid.
                                 */
    for (;;)
    {
        // Choose a PID from the list (in a round-robin scheme).
        ++pindex;
        if ( pindex >= pAIConfig->pids.size() ) pindex = 0;
        const std::string pid = pAIConfig->pids[pindex];
        
        // Run the AI Player under the chosen PID.
        try
        {
            hoxLog(LOG_INFO, "%s: AI [%s] STARTED.", __FUNCTION__, pid.c_str());
            AIPlayer_SPtr player( new AIPlayer(pid, pAIConfig->password, pAIConfig->role) );

            if ( ! player->loadAIEngine( pAIConfig->engine, pAIConfig->depth ) )
            {
                hoxLog(LOG_ERROR, "%s: Failed to load AI [%s].", __FUNCTION__, pAIConfig->engine.c_str());
                break;   // get out of here!!!
            }

            player->run();
            hoxLog(LOG_INFO, "%s: AI [%s] ENDED.", __FUNCTION__, pid.c_str());

            // Wait for a while before logging in again.
            st_sleep( 60 /* in seconds */ );
        }
        catch (std::runtime_error& ex)
        {
            hoxLog(LOG_WARN, "%s: Caught runtime exception [%s].", __FUNCTION__, ex.what());
        }
    }

    hoxLog(LOG_INFO, "%s: [%s] END.", __FUNCTION__, sPIDs.c_str());
    return NULL;
}

/******************* END OF FILE *********************************************/
