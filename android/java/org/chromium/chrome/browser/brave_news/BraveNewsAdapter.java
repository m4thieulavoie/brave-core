package org.chromium.chrome.browser.brave_news;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.graphics.drawable.Drawable;
import android.graphics.drawable.LayerDrawable;
import android.graphics.drawable.ShapeDrawable;
import android.graphics.drawable.shapes.RoundRectShape;
import android.net.Uri;
import android.os.Build;
import android.text.TextUtils;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;
import androidx.recyclerview.widget.RecyclerView;

import org.chromium.chrome.R;
import org.chromium.chrome.browser.brave_news.models.NewsItem;
import org.chromium.chrome.browser.ntp.BraveNewTabPageLayout;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

public class BraveNewsAdapter extends RecyclerView.Adapter<BraveNewsAdapter.ViewHolder> {

    private List<String> mData;
    private LayoutInflater mInflater;
    public RecycleItemClickListener mClickListener;
    private int mType;
    Context mContext;
    Activity mActivity;
    View view;
    LinearLayout linearLayout;
    LinearLayout.LayoutParams linearLayoutParams;
    CopyOnWriteArrayList<NewsItem> mNewsItems;

    int cardPosition = 0;
    private boolean isOptIn = false;

    private final int CARD_LAYOUT = 0;
    private final int BUTTON_LAYOUT = 1;

    private final int HEADLINE = 0;
    private final int HEADLINEPAIR = 1;
    private final int THREE_ROWS_PHOTO = 2;
    private final int TOP_NEWS = 3;
    private final int THREE_ROWS_HEADLINES = 4;
    private final int DISPLAY_AD = 5;
    private final int DEALS = 6;
    private final int WELCOME = 7;

    ViewHolder mHolder;
    private final String TAG = "BNA";


    public BraveNewsAdapter(Activity activity, CopyOnWriteArrayList<NewsItem> newsItems) {
        this.mInflater = LayoutInflater.from(activity);
        this.mActivity = activity;
        this.mNewsItems = newsItems;
    }
    // data is passed into the constructor

    private int getHeight() {
        DisplayMetrics displayMetrics = new DisplayMetrics();
        mActivity.getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
        int height = displayMetrics.heightPixels;

        return height;
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        view = mInflater.inflate(R.layout.brave_news_row, parent, false);
        mHolder = new ViewHolder(view);
        return mHolder;
    }

    @RequiresApi(api = Build.VERSION_CODES.JELLY_BEAN_MR1)
    @Override
    public void onBindViewHolder(@NonNull BraveNewsAdapter.ViewHolder holder, int position) {

        if (position >= getItemCount() - 10) {
            Log.d(TAG, "end");
            return;
        }
        LinearLayout.LayoutParams params1;

//        Log.d(TAG, "optedin:" + isOptIn);

//        if (!isOptIn) {
//            createCard(WELCOME, position);
//        } else
//            {
//            linearLayoutParams.setMargins(40, 0, 40, 40);
        switch (cardPosition) {
            case 1:
                //TOP news
                Log.d(TAG, "creating TOP_NEWS");
                new CardBuilder(mHolder.linearLayout, mActivity, position, mNewsItems.get(position), TOP_NEWS);
                break;
            case 2:
                Log.d(TAG, "creating HEADLINE");
                new CardBuilder(mHolder.linearLayout, mActivity, position, mNewsItems.get(position), HEADLINE);
                // Display Ad
                break;
            case 3:
            case 4:
                /* headline

                      Image
                    -------------
                    |           |
                    |           |
                    -------------
                      Title
                      Description

                */
                new CardBuilder(mHolder.linearLayout, mActivity, position, mNewsItems.get(position), HEADLINE);
                break;
            case 5:
                /*headlinepair

                      Image      Image
                    ---------    ---------
                    |       |    |       |
                    |       |    |       |
                    ---------    ---------
                      Title        Title
                   Description    Description

                 */
                new CardBuilder(mHolder.linearLayout, mActivity, position, mNewsItems.get(position), HEADLINEPAIR);

                break;
            case 6:
                // promoted content
                new CardBuilder(mHolder.linearLayout, mActivity, position, mNewsItems.get(position), HEADLINE);
                break;
            case 7:
                // 3x Headlines
                new CardBuilder(mHolder.linearLayout, mActivity, position, mNewsItems.get(position), THREE_ROWS_HEADLINES);
                break;
            case 8:
            case 9:
                new CardBuilder(mHolder.linearLayout, mActivity, position, mNewsItems.get(position), DEALS);
                // headline
                break;
            case 10:
            case 11:
                // 2 col headline
                break;
            case 12:
                // Display Ad
                break;
            case 13:
            case 14:
                // Headline
                break;
            case 15:
                // Publisher card (3x cards)
                break;
            case 16:
                // List card (3x cards)
                break;
            case 17:
                // Headline
                break;
            case 18:
                // Offers card
                break;
            case 19:
                //Headline
                break;
            case 20:
                // 2 col Headline
                break;
        }
        if (position % 30 == 0) {
            cardPosition = 0;
        }
        cardPosition++;
    }


    @Override
    public int getItemCount() {
        return mNewsItems.size();
    }

    private void initHolderViews() {
        mHolder.layou1 = new LinearLayout(mActivity);
        mHolder.layoutLeft = new LinearLayout(mActivity);
        mHolder.layoutRight = new LinearLayout(mActivity);
        mHolder.title = new TextView(mActivity);
        mHolder.desc = new TextView(mActivity);

        mHolder.image = new ImageView(mActivity);
        mHolder.logo = new ImageView(mActivity);
        mHolder.source = new TextView(mActivity);
        mHolder.layoutRowPhotos = new TableRow(mActivity);
        mHolder.layoutRow = new TableRow(mActivity);
        mHolder.row1 = new TableRow(mActivity);
        mHolder.row2 = new TableRow(mActivity);
        mHolder.row3 = new TableRow(mActivity);
        mHolder.layout1 = new LinearLayout(mActivity);
        mHolder.layout2 = new LinearLayout(mActivity);
        mHolder.layout3 = new LinearLayout(mActivity);
        mHolder.layout = new LinearLayout(mActivity);

        mHolder.tableLayoutTopNews = new TableLayout(mActivity);

        mHolder.rowTop = new TableRow(mActivity);
        mHolder.topText = new TextView(mActivity);

    }

    public void setClickListener(BraveNewTabPageLayout recyclerMain) {
        this.mClickListener = recyclerMain;
    }

    // stores and recycles views as they are scrolled off screen
    public class ViewHolder extends RecyclerView.ViewHolder implements View.OnClickListener {
        LinearLayout layoutRight;
        LinearLayout layoutLeft;
        TextView myTextView;
        TextView testTextView;
        LinearLayout testLinear;

        TableRow rowTop;
        TextView topText;

        ImageView image;
        ImageView logo;
        TextView title;
        TextView source;
        TextView desc;
        LinearLayout layou1;

        TableRow layoutRowPhotos;
        TableRow layoutRow;

        TableRow row1;
        TableRow row2;
        TableRow row3;

        LinearLayout layout1;
        LinearLayout layout2;
        LinearLayout layout3;

        LinearLayout layout;

        TableLayout tableLayoutTopNews;

        LinearLayout linearLayout;

        ViewHolder(View itemView) {
            super(itemView);
            myTextView = itemView.findViewById(R.id.item);
            testTextView = new TextView(mActivity);
            testLinear = new LinearLayout(mActivity);

            layou1 = new LinearLayout(mActivity);
            title = new TextView(mActivity);
            desc = new TextView(mActivity);

            image = new ImageView(mActivity);
            logo = new ImageView(mActivity);
            source = new TextView(mActivity);
            layoutRowPhotos = new TableRow(mActivity);
            layoutRow = new TableRow(mActivity);
            row1 = new TableRow(mActivity);
            row2 = new TableRow(mActivity);
            row3 = new TableRow(mActivity);
            layout1 = new LinearLayout(mActivity);
            layout2 = new LinearLayout(mActivity);
            layout3 = new LinearLayout(mActivity);
            layout = new LinearLayout(mActivity);

            tableLayoutTopNews = new TableLayout(mActivity);

            rowTop = new TableRow(mActivity);
            topText = new TextView(mActivity);


            itemView.setOnClickListener(this);
            linearLayout = itemView.findViewById(R.id.card_layout);

            linearLayoutParams = new LinearLayout.LayoutParams(LinearLayout.LayoutParams.MATCH_PARENT, LinearLayout.LayoutParams.WRAP_CONTENT);
            linearLayoutParams.setMargins(0, 40, 0, 40);
        }

        @Override
        public void onClick(View view) {
            if (mClickListener != null) mClickListener.onItemClick(view, getAdapterPosition());
        }
    }



    public NewsItem getItem(int id) {
        return mNewsItems.get(id);
    }

    public interface RecycleItemClickListener {
        void onItemClick(View view, int position);

        void onOptInClick(View view);

        void onCloseClick(View view);
    }

}
